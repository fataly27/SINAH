// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MousePlayerController.h"

AMousePlayerController::AMousePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/FogOfWarDecal.FogOfWarDecal"));
	FogOfWarMaterial = MaterialAsset.Object;
}

// Called when the game starts or when spawned
void AMousePlayerController::BeginPlay()
{
	HUD = Cast<APlayerHUD>(GetHUD());
	PlayerIsBlue = Role == ROLE_Authority;

	if (HUD)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMousePlayerController::FogOfWar, 0.4f, true);

		FogOfWarTexture = UTexture2D::CreateTransient(MidTextureSize * 2, MidTextureSize * 2);
		FogOfWarTexture->UpdateResource();
		TextureRegions = new FUpdateTextureRegion2D(0, 0, 0, 0, MidTextureSize * 2, MidTextureSize * 2);
		TextureData.Init(FColor(0, 0, 0, 255), MidTextureSize * MidTextureSize * 4);

		DynFOWMaterial = UMaterialInstanceDynamic::Create(FogOfWarMaterial, this);

		TActorIterator<ADecalActor> ActorItr(GetWorld());
		ActorItr->SetDecalMaterial(DynFOWMaterial);
	}
}

void AMousePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	MyPawn = Cast<AMainCamera>(InPawn);
}

// Called every frame
void AMousePlayerController::Tick(float DeltaTime)
{
	if (HUD)
	{
		float LocationX;
		float LocationY;
		GetMousePosition(LocationX, LocationY);
		HUD->SetCurrentMousePos(FVector2D(LocationX, LocationY));

		if (BoxDisplayed == TypeBox::Select)
			UpdateBoxSelection(HUD->GetActorsBeingSelected());
		else if (BoxDisplayed == TypeBox::Target)
			UpdateBoxTargeting(HUD->GetActorsBeingSelected(), false);

		TArray<TScriptInterface<IGameElementInterface>> AllActorsSelected;
		for (int i = 0; i < ActorsSelected.Num(); i++)
		{
			if ((ActorsSelected[i]->TellIfImBlue() == PlayerIsBlue || ActorsSelected[i]->GetOpponentVisibility()) && !ActorsSelected[i]->IsPendingKill())
				AllActorsSelected.Add(ActorsSelected[i]);
			else
				ActorsSelected[i]->Unselect();
		}
		ActorsSelected = AllActorsSelected;

		TArray<TScriptInterface<IGameElementInterface>> NewActorsSelectedByBox;
		for (int i = 0; i < ActorsSelectedByCurrentBox.Num(); i++)
		{
			if ((ActorsSelectedByCurrentBox[i]->TellIfImBlue() == PlayerIsBlue || ActorsSelectedByCurrentBox[i]->GetOpponentVisibility()) && !ActorsSelectedByCurrentBox[i]->IsPendingKill())
				NewActorsSelectedByBox.Add(ActorsSelectedByCurrentBox[i]);
			else
				ActorsSelectedByCurrentBox[i]->Unselect();
		}
		ActorsSelectedByCurrentBox = NewActorsSelectedByBox;

		AllActorsSelected.Append(ActorsSelectedByCurrentBox);
		HUD->SetActorsSelected(AllActorsSelected);

		if (MyPawn)
			HUD->SetZoom(MyPawn->GetZoom());
	}
}

// Called to bind functionality to input
void AMousePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Hook up every-frame handling for our four axes
	InputComponent->BindAction("Select", IE_Pressed, this, &AMousePlayerController::StartSelect);
	InputComponent->BindAction("AddSelect", IE_Pressed, this, &AMousePlayerController::StartAddSelect);
	InputComponent->BindAction("Select", IE_Released, this, &AMousePlayerController::Select);
	InputComponent->BindAction("AddSelect", IE_Released, this, &AMousePlayerController::AddSelect);
	InputComponent->BindAction("Direct", IE_Pressed, this, &AMousePlayerController::StartDirect);
	InputComponent->BindAction("AddDirect", IE_Pressed, this, &AMousePlayerController::StartAddDirect);
	InputComponent->BindAction("Direct", IE_Released, this, &AMousePlayerController::Direct);
	InputComponent->BindAction("AddDirect", IE_Released, this, &AMousePlayerController::AddDirect);

	InputComponent->BindAxis("MoveForward", this, &AMousePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMousePlayerController::MoveRight);
	InputComponent->BindAxis("Zoom", this, &AMousePlayerController::Zoom);

}

//Input functions
void AMousePlayerController::MoveForward(float AxisValue)
{
	if (MyPawn)
		MyPawn->SetMovementInputX(AxisValue);
}
void AMousePlayerController::MoveRight(float AxisValue)
{
	if (MyPawn)
		MyPawn->SetMovementInputY(AxisValue);
}
void AMousePlayerController::Zoom(float AxisValue)
{
	if (MyPawn)
		MyPawn->SetZoom(AxisValue);
}

void AMousePlayerController::StartSelect()
{
	if (BoxDisplayed != TypeBox::Target)
	{
		ClearSelection();
		StartAddSelect();
	}
}
void AMousePlayerController::StartAddSelect()
{
	if (BoxDisplayed != TypeBox::Target)
	{
		float LocationX;
		float LocationY;
		GetMousePosition(LocationX, LocationY);
		FVector2D StartMousePos;
		StartMousePos.X = LocationX;
		StartMousePos.Y = LocationY;

		HUD->SetStartMousePos(StartMousePos);
		HUD->IsPlayerBlue(PlayerIsBlue);
		HUD->ShouldDisplayBox(TypeBox::Select);

		BoxDisplayed = TypeBox::Select;
	}
}
void AMousePlayerController::Select()
{
	AddSelect();
}
void AMousePlayerController::AddSelect()
{
	if (BoxDisplayed == TypeBox::Select)
	{
		UpdateBoxSelection(HUD->GetActorsBeingSelected());
		ActorsSelected.Append(ActorsSelectedByCurrentBox);

		ActorsSelectedByCurrentBox.Empty();
		HUD->ShouldDisplayBox(TypeBox::None);
		BoxDisplayed = TypeBox::None;
	}
}
void AMousePlayerController::StartDirect()
{
	if (BoxDisplayed != TypeBox::Select && ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
	{
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->TellIfImBlue() == PlayerIsBlue)
			{
				if (PlayerIsBlue)
				{
					CurrentUnit->ClearDestinations();
					CurrentUnit->ClearSpecialTargets();
				}
				else
				{
					Server_ClearDestinations(CurrentUnit);
					Server_ClearSpecialTargets(CurrentUnit);
				}
			}
		}
		StartAddDirect();
	}
}
void AMousePlayerController::StartAddDirect()
{
	if (BoxDisplayed != TypeBox::Select)
	{
		float LocationX;
		float LocationY;
		GetMousePosition(LocationX, LocationY);
		FVector2D StartMousePos;
		StartMousePos.X = LocationX;
		StartMousePos.Y = LocationY;

		HUD->SetStartMousePos(StartMousePos);
		HUD->IsPlayerBlue(PlayerIsBlue);
		HUD->ShouldDisplayBox(TypeBox::Target);

		BoxDisplayed = TypeBox::Target;
	}
}
void AMousePlayerController::Direct()
{
	AddDirect();
}
void AMousePlayerController::AddDirect()
{
	if (BoxDisplayed == TypeBox::Target  && ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
	{
		TArray<TScriptInterface<IGameElementInterface>> NewTargets = HUD->GetActorsBeingSelected();
		UpdateBoxTargeting(NewTargets, true);

		if (NewTargets.Num() == 0 && HUD->IsBoxTiny())
		{
			FHitResult HitResult;
			GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, HitResult);

			if (HitResult.bBlockingHit)
			{
				//Creates a formation with the units

				TArray<AUnit*> Units;
				FVector Direction(0.f);
				for (int i(0); i < ActorsSelected.Num(); i++)
				{
					AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
					if (CurrentUnit->TellIfImBlue() == PlayerIsBlue && CurrentUnit->GetMode() != Modes::Defense)
					{
						Units.Add(CurrentUnit);
						Direction += HitResult.ImpactPoint - CurrentUnit->GetLocationAfterAllMoves();
					}
				}
				Direction.Normalize();
				FRotator Rotation = Direction.Rotation();

				int NumberUnits = Units.Num();
				int NumberLines = FMath::RoundToZero(FGenericPlatformMath::Sqrt(NumberUnits + 1)) - 1;
				if (NumberLines == 0)
					NumberLines++;
				int NumberColumns = FMath::DivideAndRoundUp(NumberUnits, NumberLines);

				float MidWidth = 250.f * (NumberColumns - 1) / 2;

				FVector2D Position(0.f, MidWidth);
				int k = 0;
				for (int i(0); i < NumberLines; i++)
				{
					if (i == (NumberLines - 1) && NumberUnits % NumberLines != 0)
					{
						NumberColumns = NumberUnits % NumberColumns;
						MidWidth = 250.f * (NumberColumns - 1) / 2;
					}
					Position.Y = MidWidth;

					for (int j(0); j < NumberColumns; j++)
					{
						if (PlayerIsBlue)
							Units[k]->AddDestination(HitResult.ImpactPoint + FVector(Position.GetRotated(Rotation.Yaw), 0.f), Rotation);
						else
							Server_AddDestination(Units[k], HitResult.ImpactPoint + FVector(Position.GetRotated(Rotation.Yaw), 0.f), Rotation);

						Position.Y -= 250.f;
						k++;
					}

					Position.X -= 250.f;
				}
			}
		}

		HUD->ShouldDisplayBox(TypeBox::None);
		BoxDisplayed = TypeBox::None;
	}
}

void AMousePlayerController::ClearSelection()
{
	for (int i(0); i < ActorsSelected.Num(); i++)
	{
		ActorsSelected[i]->Unselect();
	}
	ActorsSelected.Empty();
}
void AMousePlayerController::UpdateBoxSelection(TArray<TScriptInterface<IGameElementInterface>> NewSelection)
{
	TArray<TScriptInterface<IGameElementInterface>> AllSelectedBefore(ActorsSelected);
	AllSelectedBefore.Append(ActorsSelectedByCurrentBox);
	TArray<TScriptInterface<IGameElementInterface>> FinalSelection;

	// Removing doubles
	for (int i(0); i < ActorsSelected.Num(); i++)
	{
		int index = NewSelection.Find(ActorsSelected[i]);
		if (index != INDEX_NONE)
			NewSelection.RemoveAt(index);
	}

	//Unselection and selection
	for (int i(0); i < ActorsSelectedByCurrentBox.Num(); i++)
	{
		ActorsSelectedByCurrentBox[i]->Unselect();
	}

	for (int i(0); i < NewSelection.Num(); i++)
	{
		//Si le nouvel acteur sélectionné est une unité
		if (Cast<AActor>(NewSelection[i].GetObject())->IsA(AUnit::StaticClass()))
		{
			// Si le premier acteur sélectionné est un bâtiment, on le supprime de la sélection
			if (ActorsSelected.IsValidIndex(0) && Cast<AActor>(ActorsSelected.Top().GetObject())->IsA(ABuilding::StaticClass()))
			{
				ActorsSelected[0]->Unselect();
				ActorsSelected.RemoveAt(0);
			}
			else if (FinalSelection.IsValidIndex(0) && Cast<AActor>(FinalSelection.Top().GetObject())->IsA(ABuilding::StaticClass()))
				FinalSelection.RemoveAt(0);

			//Si la nouvelle unité sélectionnée est alliée
			if (NewSelection[i]->TellIfImBlue() == PlayerIsBlue)
			{
				// Si le premier acteur sélectionné est une unité ennemie, on le supprime de la sélection
				if (ActorsSelected.IsValidIndex(0) && ActorsSelected.Top()->TellIfImBlue() != PlayerIsBlue)
				{
					ActorsSelected[0]->Unselect();
					ActorsSelected.RemoveAt(0);
				}
				else if (FinalSelection.IsValidIndex(0) && FinalSelection.Top()->TellIfImBlue() != PlayerIsBlue)
					FinalSelection.RemoveAt(0);

				//On ajoute la nouvelle unité
				FinalSelection.Add(NewSelection[i]);
			}
			//Si la nouvelle unité sélectionnée est ennemie, l'ancienne sélection doit être vide pour que cette unité soit sélectionnée
			else if (FinalSelection.Num() == 0)
			{
				if (AllSelectedBefore.IsValidIndex(0))
				{
					if (AllSelectedBefore[0] == NewSelection[i])
						FinalSelection.Add(NewSelection[i]);
				}
				else
					FinalSelection.Add(NewSelection[i]);
			}
		}
		//Si le nouvel acteur sélectionné est un bâtiment, l'ancienne sélection doit être vide pour que ce bâtiment soit sélectionné
		else if (FinalSelection.Num() == 0)
		{
			if (AllSelectedBefore.IsValidIndex(0))
			{
				if(AllSelectedBefore[0] == NewSelection[i])
					FinalSelection.Add(NewSelection[i]);
			}
			else
				FinalSelection.Add(NewSelection[i]);
		}
	}

	for (int i(0); i < FinalSelection.Num(); i++)
	{
		FinalSelection[i]->Select();
	}
	ActorsSelectedByCurrentBox = FinalSelection;
}
void AMousePlayerController::UpdateBoxTargeting(TArray<TScriptInterface<IGameElementInterface>> NewTargets, bool IsFinal)
{
	if (ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
	{
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->TellIfImBlue() == PlayerIsBlue)
			{
				if (Role == ROLE_Authority)
					CurrentUnit->SetBoxSpecialTargets(NewTargets);
				else
				{
					TArray<AActor*> UpdatedNewTargets;
					for (int j(0); j < NewTargets.Num(); j++)
					{
						UpdatedNewTargets.Add(Cast<AActor>(NewTargets[j].GetObject()));
					}

					Server_SetBoxSpecialTargets(CurrentUnit, UpdatedNewTargets);
				}
				if (IsFinal)
				{
					if (Role == ROLE_Authority)
						CurrentUnit->AddSpecialTargets();
					else
						Server_AddSpecialTargets(CurrentUnit);
				}
			}
		}
	}
}

void AMousePlayerController::FogOfWar()
{
	TArray<TScriptInterface<IGameElementInterface>> BlueActors;
	TArray<TScriptInterface<IGameElementInterface>> RedActors;

	TActorIterator<AUnit> UnitItr(GetWorld());
	for (UnitItr; UnitItr; ++UnitItr)
	{
		if (UnitItr->TellIfImBlue())
			BlueActors.Add(TScriptInterface<IGameElementInterface>(*UnitItr));
		else
			RedActors.Add(TScriptInterface<IGameElementInterface>(*UnitItr));
	}

	TActorIterator<ABuilding> BuildingItr(GetWorld());
	for (BuildingItr; BuildingItr; ++BuildingItr)
	{
		if (BuildingItr->TellIfImBlue())
			BlueActors.Add(TScriptInterface<IGameElementInterface>(*BuildingItr));
		else
			RedActors.Add(TScriptInterface<IGameElementInterface>(*BuildingItr));
	}

	bool ShouldBeVisible(false);
	if (Role == ROLE_Authority)
	{
		for (int i = 0; i < BlueActors.Num(); i++)
		{
			ShouldBeVisible = false;
			TArray<TScriptInterface<IGameElementInterface>> OpponentsInSight;
			for (int j = 0; j < RedActors.Num(); j++)
			{
				FVector vecteur = RedActors[j]->GetLocation() - BlueActors[i]->GetLocation();
				float temp = vecteur.Size();
				if (FVector(RedActors[j]->GetLocation() - BlueActors[i]->GetLocation()).Size() <= RedActors[j]->GetFieldOfSight() * 100.f)
					ShouldBeVisible = true;
				if (FVector(RedActors[j]->GetLocation() - BlueActors[i]->GetLocation()).Size() <= BlueActors[i]->GetFieldOfSight() * 100.f)
					OpponentsInSight.Add(RedActors[j]);
			}
			if (BlueActors[i].GetObject()->IsA(AUnit::StaticClass()))
			{
				AUnit* BlueUnit = Cast<AUnit>(BlueActors[i].GetObject());

				if (BlueUnit->GetOpponentVisibility() != ShouldBeVisible)
					BlueUnit->Multicast_SetHidden(!ShouldBeVisible, BlueUnit->GetLocation(), BlueUnit->GetActorRotation());
				BlueUnit->SetOpponentsInSight(OpponentsInSight);
			}
		}
		for (int i = 0; i < RedActors.Num(); i++)
		{
			ShouldBeVisible = false;
			TArray<TScriptInterface<IGameElementInterface>> OpponentsInSight;
			for (int j = 0; j < BlueActors.Num(); j++)
			{
				if (FVector(BlueActors[j]->GetLocation() - RedActors[i]->GetLocation()).Size() <= BlueActors[j]->GetFieldOfSight() * 100.f)
					ShouldBeVisible = true;
				if (FVector(BlueActors[j]->GetLocation() - RedActors[i]->GetLocation()).Size() <= RedActors[i]->GetFieldOfSight() * 100.f)
					OpponentsInSight.Add(BlueActors[j]);
			}

			if (RedActors[i].GetObject()->IsA(AUnit::StaticClass()))
			{
				AUnit* RedUnit = Cast<AUnit>(RedActors[i].GetObject());

				if (RedUnit->GetOpponentVisibility() != ShouldBeVisible)
					RedUnit->Multicast_SetHidden(!ShouldBeVisible, RedUnit->GetLocation(), RedUnit->GetActorRotation());
				RedUnit->SetOpponentsInSight(OpponentsInSight);
			}
		}
	}

	SetFogOfWarTexture(BlueActors, RedActors);
}
void AMousePlayerController::SetFogOfWarTexture(TArray<TScriptInterface<IGameElementInterface>> BlueUnits, TArray<TScriptInterface<IGameElementInterface>> RedUnits)
{
	if (FogOfWarTexture)
	{
		int Width, Height;
		GetViewportSize(Width, Height);
		FVector WorldLocation, WorldDirection;
		float NumberOfDirectionToAdd;

		FVector TopLeft, TopRight, BottomLeft, BottomRight;

		DeprojectScreenPositionToWorld(0, 0, WorldLocation, WorldDirection);
		NumberOfDirectionToAdd = -WorldLocation.Z / WorldDirection.Z;
		TopLeft = FVector(WorldLocation.X + WorldDirection.X * NumberOfDirectionToAdd, WorldLocation.Y + WorldDirection.Y * NumberOfDirectionToAdd, 0.f);

		DeprojectScreenPositionToWorld(Width - 1, 0, WorldLocation, WorldDirection);
		NumberOfDirectionToAdd = -WorldLocation.Z / WorldDirection.Z;
		TopRight = FVector(WorldLocation.X + WorldDirection.X * NumberOfDirectionToAdd, WorldLocation.Y + WorldDirection.Y * NumberOfDirectionToAdd, 0.f);

		DeprojectScreenPositionToWorld(0, Height - 1, WorldLocation, WorldDirection);
		NumberOfDirectionToAdd = -WorldLocation.Z / WorldDirection.Z;
		BottomLeft = FVector(WorldLocation.X + WorldDirection.X * NumberOfDirectionToAdd, WorldLocation.Y + WorldDirection.Y * NumberOfDirectionToAdd, 0.f);

		DeprojectScreenPositionToWorld(Width - 1, Height - 1, WorldLocation, WorldDirection);
		NumberOfDirectionToAdd = -WorldLocation.Z / WorldDirection.Z;
		BottomRight = FVector(WorldLocation.X + WorldDirection.X * NumberOfDirectionToAdd, WorldLocation.Y + WorldDirection.Y * NumberOfDirectionToAdd, 0.f);

		float PixelRatio = MidTerrainSize / MidTextureSize;
		float MinX = std::min({ TopLeft.X , TopRight.X, BottomLeft.X, BottomRight.X });(TopLeft.X <= BottomRight.X);
		float MaxX = std::max({ TopLeft.X , TopRight.X, BottomLeft.X, BottomRight.X });(TopLeft.X >= BottomRight.X);
		float MinY = std::min({ TopLeft.Y , TopRight.Y, BottomLeft.Y, BottomRight.Y });(TopLeft.Y <= BottomRight.Y);
		float MaxY = std::max({ TopLeft.Y , TopRight.Y, BottomLeft.Y, BottomRight.Y });(TopLeft.Y >= BottomRight.Y);

		int PixelMinX = MinX / PixelRatio;
		int PixelMaxX = MaxX / PixelRatio;
		int PixelMinY = MinY / PixelRatio;
		int PixelMaxY = MaxY / PixelRatio;

		PixelMinX -= 50;
		PixelMaxX += 50;
		PixelMinY -= 50;
		PixelMaxY += 50;

		if (PixelMinX < -MidTextureSize)
			PixelMinX = -MidTextureSize;
		if (PixelMaxX > MidTextureSize)
			PixelMaxX = MidTextureSize;
		if (PixelMinY < -MidTextureSize)
			PixelMinY = -MidTextureSize;
		if (PixelMaxY > MidTextureSize)
			PixelMaxY = MidTextureSize;

		TArray<FVector> UnitsPosition;
		TArray<float> UnitsSight;

		if (PlayerIsBlue)
		{
			for (int i(0); i < BlueUnits.Num(); i++)
			{
				FVector TempPosition = BlueUnits[i]->GetLocation();
				UnitsPosition.Add(FVector(TempPosition.X, TempPosition.Y, 0.f));
				UnitsSight.Add(BlueUnits[i]->GetFieldOfSight());
			}
		}
		else
		{
			for (int i(0); i < RedUnits.Num(); i++)
			{
				FVector TempPosition = RedUnits[i]->GetLocation();
				UnitsPosition.Add(FVector(TempPosition.X, TempPosition.Y, 0.f));
				UnitsSight.Add(RedUnits[i]->GetFieldOfSight());
			}
		}

		int NumUnits = UnitsPosition.Num();

		for (int y(PixelMinY); y < PixelMaxY; y++)
		{
			for (int x(PixelMinX); x < PixelMaxX; x++)
			{
				FVector PositionPixel(x * PixelRatio, y * PixelRatio, 0.f);

				int Alpha = 255;
				for (int i(0); i < NumUnits; i++)
				{
					int Difference = UnitsSight[i] * 100.f - (PositionPixel - UnitsPosition[i]).Size2D();
					if (Difference >= 0)
					{
						if (Difference >= 255)
						{
							Alpha = 0;
							break;
						}
						else if (255 - Difference < Alpha)
							Alpha = 255 - Difference;
					}
				}
				TextureData[(y + MidTextureSize) * MidTextureSize * 2 + x + MidTextureSize] = FColor(0, 0, 0, Alpha);
			}
		}

		UpdateTextureRegions(FogOfWarTexture, (int32)0, (uint32)1, TextureRegions, (uint32)(4 * MidTextureSize * 2), (uint32)4, (uint8*)TextureData.GetData(), false);
		DynFOWMaterial->SetTextureParameterValue("TextureParameter", FogOfWarTexture);
	}
}
void AMousePlayerController::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

void AMousePlayerController::Server_AddDestination_Implementation(AUnit *Unit, FVector Destination, FRotator Rotation)
{
	Unit->AddDestination(Destination, Rotation);
}
bool AMousePlayerController::Server_AddDestination_Validate(AUnit *Unit, FVector Destination, FRotator Rotation)
{
	return Unit != NULL && !Unit->TellIfImBlue() && !Unit->IsPendingKill();
}
void AMousePlayerController::Server_ClearDestinations_Implementation(AUnit *Unit)
{
	Unit->ClearDestinations();
}
bool AMousePlayerController::Server_ClearDestinations_Validate(AUnit *Unit)
{
	return Unit != NULL && !Unit->TellIfImBlue() && !Unit->IsPendingKill();
}

void AMousePlayerController::Server_AddSpecialTargets_Implementation(AUnit *Unit)
{
	Unit->AddSpecialTargets();
}
bool AMousePlayerController::Server_AddSpecialTargets_Validate(AUnit *Unit)
{
	return Unit != NULL && !Unit->TellIfImBlue() && !Unit->IsPendingKill();
}
void AMousePlayerController::Server_SetBoxSpecialTargets_Implementation(AUnit *Unit, const TArray<AActor*> &NewTargets)
{
	TArray<TScriptInterface<IGameElementInterface>> UpdatedNewTargets;
	for (int i(0); i < NewTargets.Num(); i++)
	{
		TScriptInterface<IGameElementInterface> NewInterface;
		NewInterface.SetInterface(Cast<IGameElementInterface>(NewTargets[i]));
		NewInterface.SetObject(NewTargets[i]);

		UpdatedNewTargets.Add(NewInterface);
	}

	Unit->SetBoxSpecialTargets(UpdatedNewTargets);
}
bool AMousePlayerController::Server_SetBoxSpecialTargets_Validate(AUnit *Unit, const TArray<AActor*> &NewTargets)
{
	return Unit != NULL && !Unit->TellIfImBlue() && !Unit->IsPendingKill();
}
void AMousePlayerController::Server_ClearSpecialTargets_Implementation(AUnit *Unit)
{
	Unit->ClearSpecialTargets();
}
bool AMousePlayerController::Server_ClearSpecialTargets_Validate(AUnit *Unit)
{
	return Unit != NULL && !Unit->TellIfImBlue() && !Unit->IsPendingKill();
}