// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Blueprint/UserWidget.h"
#include "Buildings/MilitaryBuilding.h"
#include "MultiplayerSinahMode.h"
#include "MultiplayerGameState.h"
#include "MultiplayerState.h"
#include "MainCamera.h"
#include "Units/Knight.h"
#include "Buildings/Zones/SpeedZone.h"
#include "Buildings/Zones/LifeZone.h"

#include "Buildings/FoodEconomicBuilding.h"
#include "Buildings/CristalsEconomicBuilding.h"
#include "Buildings/CellsEconomicBuilding.h"
#include "Buildings/MetalEconomicBuilding.h"

#include "Widgets/UpWidget.h"
#include "Widgets/StatWidget.h"
#include "Widgets/MapWidget.h"
#include "Widgets/ModesWidget.h"
#include "Widgets/LevelEconomicWidget.h"
#include "Widgets/LevelMilitaryWidget.h"
#include "Widgets/SpawnWidget.h"
#include "Widgets/SpawnEntityWidget.h"

#include <cmath>
#include <algorithm>

#include "MousePlayerController.h"

AMousePlayerController::AMousePlayerController() : TimeSinceLastHarvest(0.f), OpponentView(false), OldString(""), IsThrobberEnabled(true), IsExitEnabled(false)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/FogOfWarDecal.FogOfWarDecal"));
	FogOfWarMaterial = MaterialAsset.Object;


	static ConstructorHelpers::FObjectFinder<UTexture> UnselectedAsset(TEXT("/Game/Textures/Modes_Buttons/button.button"));
	UnselectedTexture = UnselectedAsset.Object;

	static ConstructorHelpers::FObjectFinder<UTexture> SelectedAsset(TEXT("/Game/Textures/Modes_Buttons/selected_button.selected_button"));
	SelectedTexture = SelectedAsset.Object;

	static ConstructorHelpers::FObjectFinder<UTexture> SelectedAmidSeveralAsset(TEXT("/Game/Textures/Modes_Buttons/multiple_selected_button.multiple_selected_button"));
	SelectedAmidSeveralTexture = SelectedAmidSeveralAsset.Object;
}

// Called when the game starts or when spawned
void AMousePlayerController::BeginPlay()
{
	HUD = Cast<APlayerHUD>(GetHUD());

	AMultiplayerSinahMode* Mode = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode());

	if (Role == ROLE_Authority)
		PlayerSide = Mode->GetPlayerSide(PlayerState->PlayerId);

	if (HUD)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMousePlayerController::FogOfWar, 0.1f, true);

		FogOfWarTexture = UTexture2D::CreateTransient(MidTextureSize * 2, MidTextureSize * 2);
		FogOfWarTexture->UpdateResource();
		TextureRegions = new FUpdateTextureRegion2D(0, 0, 0, 0, MidTextureSize * 2, MidTextureSize * 2);
		TextureData.Init(FColor(0, 0, 0, 255), MidTextureSize * MidTextureSize * 4);

		DynFOWMaterial = UMaterialInstanceDynamic::Create(FogOfWarMaterial, this);

		TActorIterator<ADecalActor> ActorItr(GetWorld());
		ActorItr->SetDecalMaterial(DynFOWMaterial);

		if (wGameStartInfo)
		{
			MyGameStartInfo = CreateWidget<UUserWidget>(this, wGameStartInfo);
			if (MyGameStartInfo)
				MyGameStartInfo->AddToViewport();
		}

		if (wTopInterface)
			MyTopInterface = CreateWidget<UUpWidget>(this, wTopInterface);
		if (wStatInterface)
			MyStatInterface = CreateWidget<UStatWidget>(this, wStatInterface);
		if (wMapInterface)
			MyMapInterface = CreateWidget<UMapWidget>(this, wMapInterface);
		if (wModesInterface)
			MyModesInterface = CreateWidget<UModesWidget>(this, wModesInterface);
		if (wEconomicLevelInterface)
			MyEconomicLevelInterface = CreateWidget<ULevelEconomicWidget>(this, wEconomicLevelInterface);
		if (wMilitaryLevelInterface)
			MyMilitaryLevelInterface = CreateWidget<ULevelMilitaryWidget>(this, wMilitaryLevelInterface);
		if (wSpawnInterface)
			MySpawnInterface = CreateWidget<USpawnWidget>(this, wSpawnInterface);

		if (MyTopInterface)
			MyTopInterface->AddToViewport();
		if (MyStatInterface)
			MyStatInterface->AddToViewport();
		if (MyMapInterface)
			MyMapInterface->AddToViewport();
		if (MyModesInterface)
			MyModesInterface->AddToViewport();
		if (MyEconomicLevelInterface)
			MyEconomicLevelInterface->AddToViewport();
		if (MyMilitaryLevelInterface)
			MyMilitaryLevelInterface->AddToViewport();

		if (MySpawnInterface && wSpawnEntityInterface)
		{
			MySpawnInterface->AddToViewport();

			FLinearColor Color;

			if (GetSide() == Side::Blue)
				Color = FLinearColor(0.5f, 0.5f, 1.f, 1.f);
			else
				Color = FLinearColor(1.f, 0.5f, 0.5f, 1.f);

			TArray<TSubclassOf<AUnit>> UnitClasses;
			UnitClasses.Add(AKnight::StaticClass());
			// ...

			TArray<TSubclassOf<AUnit>> UnitClassesLevelOne;
			TArray<TSubclassOf<AUnit>> UnitClassesLevelTwo;
			TArray<TSubclassOf<AUnit>> UnitClassesLevelThree;
			TArray<TSubclassOf<AUnit>> UnitClassesLevelFour;
			TArray<TSubclassOf<AUnit>> UnitClassesLevelFive;
			TArray<TSubclassOf<AUnit>> UnitClassesLevelSix;
			TArray<TSubclassOf<AUnit>> UnitClassesLevelSeven;

			for (int i(0); i < UnitClasses.Num(); i++)
			{
				switch (Cast<AUnit>(UnitClasses[i]->GetDefaultObject())->GetBuildingLevelRequired())
				{
					case 1:
					{
						UnitClassesLevelOne.Add(UnitClasses[i]);
						break;
					}
					case 2:
					{
						UnitClassesLevelTwo.Add(UnitClasses[i]);
						break;
					}
					case 3:
					{
						UnitClassesLevelThree.Add(UnitClasses[i]);
						break;
					}
					case 4:
					{
						UnitClassesLevelFour.Add(UnitClasses[i]);
						break;
					}
					case 5:
					{
						UnitClassesLevelFive.Add(UnitClasses[i]);
						break;
					}
					case 6:
					{
						UnitClassesLevelSix.Add(UnitClasses[i]);
						break;
					}
					case 7:
					{
						UnitClassesLevelSeven.Add(UnitClasses[i]);
						break;
					}
					default:
					{
						break;
					}
				}
			}

			for (int i(0); i < UnitClassesLevelOne.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelOne[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 1, i / 5);
			}
			for (int i(0); i < UnitClassesLevelTwo.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelTwo[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 2, i / 5);
			}
			for (int i(0); i < UnitClassesLevelThree.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelThree[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 3, i / 5);
			}
			for (int i(0); i < UnitClassesLevelFour.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelFour[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 4, i / 5);
			}
			for (int i(0); i < UnitClassesLevelFive.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelFive[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 5, i / 5);
			}
			for (int i(0); i < UnitClassesLevelSix.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelSix[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 6, i / 5);
			}
			for (int i(0); i < UnitClassesLevelSeven.Num(); i++)
			{
				USpawnEntityWidget* SpawnEntity = PrepareEntity(UnitClassesLevelSeven[i]);
				SpawnEntity->SetColor(Color);
				SpawnEntity->SetMainInterface(MySpawnInterface);

				SpawnEntityWidgets.Add(SpawnEntity);

				MySpawnInterface->AddEntity(SpawnEntity, 7, i / 5);
			}

			SpawnEntityWidgets[0]->TransferData();
		}

		if (GetSide() == Side::Blue)
			SetColorToBlue();
		if (GetSide() == Side::Red)
			SetColorToRed();
	}

	Super::BeginPlay();
}

void AMousePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	MyPawn = Cast<AMainCamera>(InPawn);
}

// Called every frame
void AMousePlayerController::Tick(float DeltaTime)
{
	if (GetWorld()->GetGameState<AMultiplayerGameState>())
	{
		FString NewString(GetWorld()->GetGameState<AMultiplayerGameState>()->GetStatusInfo());

		if (OldString != NewString)
		{
			String = GetWorld()->GetGameState<AMultiplayerGameState>()->GetStatusInfo();
			SetText();
			OldString = String;
		}

		if (GetWorld()->GetGameState<AMultiplayerGameState>()->GetStatusInfo() != "Waiting for your opponent" && IsThrobberEnabled)
		{
			DisableLoading();
			IsThrobberEnabled = false;
		}

		if ((GetWorld()->GetGameState<AMultiplayerGameState>()->GetStatusInfo() == "Blue player has won !" || GetWorld()->GetGameState<AMultiplayerGameState>()->GetStatusInfo() == "Red player has won !") && !IsExitEnabled)
		{
			EnableExit();
			IsExitEnabled = true;
		}
	}

	if (Role == ROLE_Authority && GetWorld()->GetAuthGameMode() && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
	{
		TimeSinceLastHarvest += DeltaTime;

		if (TimeSinceLastHarvest >= 0.5f)
		{
			TimeSinceLastHarvest -= 0.5f;

			TActorIterator<AFoodEconomicBuilding> Food(GetWorld());
			int AmountOfFoodToAdd(0);
			for (Food; Food; ++Food)
			{
				if (Food->GetSide() == PlayerSide)
					AmountOfFoodToAdd += Food->GetOutputInHalfASecond();
			}

			TActorIterator<ACristalsEconomicBuilding> Cristals(GetWorld());
			int AmountOfCristalsToAdd(0);
			for (Cristals; Cristals; ++Cristals)
			{
				if (Cristals->GetSide() == PlayerSide)
					AmountOfCristalsToAdd += Cristals->GetOutputInHalfASecond();
			}

			TActorIterator<ACellsEconomicBuilding> Cells(GetWorld());
			int AmountOfCellsToAdd(0);
			for (Cells; Cells; ++Cells)
			{
				if (Cells->GetSide() == PlayerSide)
					AmountOfCellsToAdd += Cells->GetOutputInHalfASecond();
			}

			TActorIterator<AMetalEconomicBuilding> Metal(GetWorld());
			int AmountOfMetalToAdd(0);
			for (Metal; Metal; ++Metal)
			{
				if (Metal->GetSide() == PlayerSide)
					AmountOfMetalToAdd += Metal->GetOutputInHalfASecond();
			}

			AMultiplayerState* State = Cast<AMultiplayerState>(PlayerState);

			State->SetAmountOfFood(State->GetAmountOfFood() + AmountOfFoodToAdd);
			State->SetAmountOfMetal(State->GetAmountOfMetal() + AmountOfMetalToAdd);
			State->SetAmountOfCells(State->GetAmountOfCells() + AmountOfCellsToAdd);
			State->SetAmountOfCristals(State->GetAmountOfCristals() + AmountOfCristalsToAdd);

			int CurrentFood(State->GetAmountOfFood());
			int NeededFood(0);
			int GlobalNeededFood(0);

			TActorIterator<AUnit> UnitItr(GetWorld());
			for (UnitItr; UnitItr; ++UnitItr)
			{
				if (UnitItr->GetSide() == GetSide())
				{
					int FoodNeededByUnit(UnitItr->GetFoodEatenInHalfASecond());

					if (CurrentFood - NeededFood - FoodNeededByUnit >= 0)
						NeededFood += FoodNeededByUnit;
					else
						UnitItr->Heal(-2 * FoodNeededByUnit);

					GlobalNeededFood += FoodNeededByUnit;
				}
			}

			State->SetAmountOfFood(CurrentFood - NeededFood);

			State->SetFoodChange(2 * (AmountOfFoodToAdd - GlobalNeededFood));
			State->SetMetalChange(2 * AmountOfMetalToAdd);
			State->SetCellsChange(2 * AmountOfCellsToAdd);
			State->SetCristalsChange(2 * AmountOfCristalsToAdd);
		}
	}

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
			if ((ActorsSelected[i]->GetSide() == PlayerSide || ActorsSelected[i]->GetOpponentVisibility()) && !ActorsSelected[i]->IsPendingKill())
				AllActorsSelected.Add(ActorsSelected[i]);
			else
				ActorsSelected[i]->Unselect();
		}
		ActorsSelected = AllActorsSelected;

		TArray<TScriptInterface<IGameElementInterface>> NewActorsSelectedByBox;
		for (int i = 0; i < ActorsSelectedByCurrentBox.Num(); i++)
		{
			if ((ActorsSelectedByCurrentBox[i]->GetSide() == PlayerSide || ActorsSelectedByCurrentBox[i]->GetOpponentVisibility()) && !ActorsSelectedByCurrentBox[i]->IsPendingKill())
				NewActorsSelectedByBox.Add(ActorsSelectedByCurrentBox[i]);
			else
				ActorsSelectedByCurrentBox[i]->Unselect();
		}
		ActorsSelectedByCurrentBox = NewActorsSelectedByBox;

		AllActorsSelected.Append(ActorsSelectedByCurrentBox);
		HUD->SetActorsSelected(AllActorsSelected);

		if (MyPawn)
			HUD->SetZoom(MyPawn->GetZoom());

		AMultiplayerState* State = Cast<AMultiplayerState>(PlayerState);

		if (State && MyTopInterface)
		{
			MyTopInterface->SetFood(State->GetAmountOfFood());
			MyTopInterface->SetMetal(State->GetAmountOfMetal());
			MyTopInterface->SetCells(State->GetAmountOfCells());
			MyTopInterface->SetCristals(State->GetAmountOfCristals());

			MyTopInterface->SetFoodChange(State->GetFoodChange());
			MyTopInterface->SetMetalChange(State->GetMetalChange());
			MyTopInterface->SetCellsChange(State->GetCellsChange());
			MyTopInterface->SetCristalsChange(State->GetCristalsChange());

			MyTopInterface->SetTime(GetWorld()->GetGameState<AMultiplayerGameState>()->GetTime());
		}

		if (MyStatInterface)
		{
			FLinearColor Color(1.f, 1.f, 1.f, 1.f);

			int FinalMaxLife(0);
			int FinalCurrentLife(0);
			int FinalHeal(0);
			float FinalFieldOfSight(0.f);
			int FinalPhysicAttack(0);
			int FinalMagicAttack(0);
			int FinalPhysicDefense(0);
			int FinalMagicDefense(0);
			float FinalSpeed(0.f);
			float FinalRange(0.f);
			int FinalFoodEatenInHalfASecond(0);

			if (AllActorsSelected.IsValidIndex(0))
			{
				MyStatInterface->SetStatsVisibility(ESlateVisibility::Visible, ESlateVisibility::Hidden);

				if (AllActorsSelected[0]->GetSide() == Side::Blue)
					Color = FLinearColor(0.5f, 0.5f, 1.f, 1.f);
				else if (AllActorsSelected[0]->GetSide() == Side::Red)
					Color = FLinearColor(1.f, 0.5f, 0.5f, 1.f);
				else
					Color = FLinearColor(0.5f, 0.5f, 0.5f, 1.f);

				if (AllActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
				{
					if (MyEconomicLevelInterface && MyMilitaryLevelInterface)
					{
						MyEconomicLevelInterface->SetIsLevelVisible(false);
						MyMilitaryLevelInterface->SetIsLevelVisible(false);
					}

					if (MyModesInterface)
					{
						if(AllActorsSelected[0]->GetSide() == GetSide())
							MyModesInterface->SetModesVisibility(ESlateVisibility::Visible);
						else
							MyModesInterface->SetModesVisibility(ESlateVisibility::Hidden);

						MyModesInterface->SetAllButtonsEnabled(true);
						MyModesInterface->SetInvisibleButtonEnabled(false);

						for (int i(0); i < AllActorsSelected.Num(); i++)
						{
							AUnit* Unit = Cast<AUnit>(AllActorsSelected[i].GetObject());

							if (Unit->GetMode() == Modes::None)
							{
								MyModesInterface->SetAllButtonsEnabled(false);
								break;
							}
							else if (Unit->GetInvisibleCoolDown() <= 0.f)
								MyModesInterface->SetInvisibleButtonEnabled(true);
						}

						float InvisibleTimeRatio = 0.f;
						float InvisibleCoolDownRatio = 1.f;

						int UnitsPerMode[] = {0, 0, 0, 0, 0};

						for (int i(0); i < AllActorsSelected.Num(); i++)
						{
							AUnit* Unit = Cast<AUnit>(AllActorsSelected[i].GetObject());

							if (Unit->GetInvisibleTime() / 15.f > InvisibleTimeRatio)
								InvisibleTimeRatio = Unit->GetInvisibleTime() / 15.f;
							if (1.f - (Unit->GetInvisibleCoolDown() / 180.f) < InvisibleCoolDownRatio)
								InvisibleCoolDownRatio = 1.f - (Unit->GetInvisibleCoolDown() / 180.f);

							if (Unit->GetMode() == Modes::Attack)
								UnitsPerMode[0] += 1;
							else if (Unit->GetMode() == Modes::Defense)
								UnitsPerMode[1] += 1;
							else if (Unit->GetMode() == Modes::Movement)
								UnitsPerMode[2] += 1;
							else if (Unit->GetMode() == Modes::Alert)
								UnitsPerMode[3] += 1;
							else if (Unit->GetMode() == Modes::Invisible)
								UnitsPerMode[4] += 1;
						}

						if (InvisibleTimeRatio > 0.f)
							MyModesInterface->SetInvisibleButtonProgress(InvisibleTimeRatio);
						else
						{
							if (FMath::IsNearlyEqual(InvisibleCoolDownRatio, 1.f, 0.001f))
								MyModesInterface->SetInvisibleButtonProgress(1.001f);
							else
								MyModesInterface->SetInvisibleButtonProgress(InvisibleCoolDownRatio);
						}

						UTexture* TexturePerMode[5];

						for (int i(0); i < 5; i++)
						{
							if (UnitsPerMode[i] == 0)
								TexturePerMode[i] = UnselectedTexture;
							else if (UnitsPerMode[i] == AllActorsSelected.Num())
								TexturePerMode[i] = SelectedTexture;
							else
								TexturePerMode[i] = SelectedAmidSeveralTexture;
						}

						MyModesInterface->SetButtonsTextures(TexturePerMode[0], TexturePerMode[1], TexturePerMode[2], TexturePerMode[3], TexturePerMode[4]);
					}

					for (int i(0); i < AllActorsSelected.Num(); i++)
					{
						AUnit* Unit = Cast<AUnit>(AllActorsSelected[i].GetObject());

						FinalMaxLife += Unit->GetMaxLife();
						FinalCurrentLife += Unit->GetCurrentLife();
						FinalPhysicAttack += Unit->GetPhysicAttack();
						FinalMagicAttack += Unit->GetMagicAttack();
						FinalPhysicDefense += Unit->GetPhysicDefense();
						FinalMagicDefense += Unit->GetMagicDefense();
						FinalFoodEatenInHalfASecond += Unit->GetFoodEatenInHalfASecond();

						if (Unit->GetSpeed() < FinalSpeed || FinalSpeed == 0.f)
							FinalSpeed = Unit->GetSpeed();
						if (Unit->GetFieldOfSight() > FinalFieldOfSight)
							FinalFieldOfSight = Unit->GetFieldOfSight();
						if (Unit->GetRange() > FinalRange)
							FinalRange = Unit->GetRange();
					}

					if (MySpawnInterface)
						MySpawnInterface->SetSpawnVisibility(false);
				}
				else
				{
					ABuilding* Building = Cast<ABuilding>(AllActorsSelected[0].GetObject());

					FinalMaxLife = Building->GetMaxLife();
					FinalCurrentLife = Building->GetCurrentLife();
					FinalHeal = Building->GetHeal();
					FinalFieldOfSight = Building->GetFieldOfSight();

					MyStatInterface->SetStatsVisibility(ESlateVisibility::Hidden, ESlateVisibility::Visible);
					if (MyModesInterface)
						MyModesInterface->SetModesVisibility(ESlateVisibility::Hidden);

					if (MyEconomicLevelInterface && MyMilitaryLevelInterface)
					{
						ULevelWidget* LevelInterface;
						bool IsAnEconomicPlunderedBuilding(false);

						if (Building->IsA(AEconomicBuilding::StaticClass()))
						{
							AEconomicBuilding* EconomicBuilding = Cast<AEconomicBuilding>(Building);

							MyEconomicLevelInterface->SetIsLevelVisible(true);
							MyMilitaryLevelInterface->SetIsLevelVisible(false);

							LevelInterface = MyEconomicLevelInterface;

							int NextOutput = EconomicBuilding->GetOutputInHalfASecond();

							MyEconomicLevelInterface->SetIsPlundered(EconomicBuilding->GetIsPlundered());
							if (EconomicBuilding->GetIsPlundered())
								IsAnEconomicPlunderedBuilding = true;
							else if (EconomicBuilding->GetLevel() < EconomicBuilding->GetMaxLevel())
								NextOutput = EconomicBuilding->GetOutputForLevel(EconomicBuilding->GetLevel() + 1);

							MyEconomicLevelInterface->SetOutput(EconomicBuilding->GetOutputInHalfASecond() * 2, NextOutput * 2);

							float RelativeCurrent = (float)EconomicBuilding->GetOutputInHalfASecond() / (float)EconomicBuilding->GetOutputForLevel(EconomicBuilding->GetMaxLevel());
							float RelativeNext = (float)NextOutput / (float)EconomicBuilding->GetOutputForLevel(EconomicBuilding->GetMaxLevel());

							MyEconomicLevelInterface->SetRelativeOutput(RelativeCurrent + 0.0001f, RelativeNext + 0.0001f);

							if(Building->IsA(AFoodEconomicBuilding::StaticClass()))
								MyEconomicLevelInterface->SetBuildingToFood();
							else if (Building->IsA(ACellsEconomicBuilding::StaticClass()))
								MyEconomicLevelInterface->SetBuildingToCells();
							else if (Building->IsA(AMetalEconomicBuilding::StaticClass()))
								MyEconomicLevelInterface->SetBuildingToMetal();
							else
								MyEconomicLevelInterface->SetBuildingToCristals();

							if (MySpawnInterface)
								MySpawnInterface->SetSpawnVisibility(false);
						}
						else
						{
							AMilitaryBuilding* MilitaryBuilding = Cast<AMilitaryBuilding>(Building);

							MyEconomicLevelInterface->SetIsLevelVisible(false);
							MyMilitaryLevelInterface->SetIsLevelVisible(true);

							LevelInterface = MyMilitaryLevelInterface;

							MyMilitaryLevelInterface->SetPoints(MilitaryBuilding->GetPoints());

							MyMilitaryLevelInterface->SetPlayerLifeEffect(MilitaryBuilding->GetPlayerLifeZone()->GetCurrentEffectLevel(), MilitaryBuilding->GetPlayerLifeZone()->GetMaxEffectLevel());
							MyMilitaryLevelInterface->SetPlayerSpeedEffect(MilitaryBuilding->GetPlayerSpeedZone()->GetCurrentEffectLevel(), MilitaryBuilding->GetPlayerSpeedZone()->GetMaxEffectLevel());
							MyMilitaryLevelInterface->SetOpponentLifeEffect(MilitaryBuilding->GetOpponentLifeZone()->GetCurrentEffectLevel(), MilitaryBuilding->GetOpponentLifeZone()->GetMaxEffectLevel());
							MyMilitaryLevelInterface->SetOpponentSpeedEffect(MilitaryBuilding->GetOpponentSpeedZone()->GetCurrentEffectLevel(), MilitaryBuilding->GetOpponentSpeedZone()->GetMaxEffectLevel());

							MyMilitaryLevelInterface->SetPlayerLifeReach(MilitaryBuilding->GetPlayerLifeZone()->GetCurrentReachLevel(), MilitaryBuilding->GetPlayerLifeZone()->GetMaxReachLevel());
							MyMilitaryLevelInterface->SetPlayerSpeedReach(MilitaryBuilding->GetPlayerSpeedZone()->GetCurrentReachLevel(), MilitaryBuilding->GetPlayerSpeedZone()->GetMaxReachLevel());
							MyMilitaryLevelInterface->SetOpponentLifeReach(MilitaryBuilding->GetOpponentLifeZone()->GetCurrentReachLevel(), MilitaryBuilding->GetOpponentLifeZone()->GetMaxReachLevel());
							MyMilitaryLevelInterface->SetOpponentSpeedReach(MilitaryBuilding->GetOpponentSpeedZone()->GetCurrentReachLevel(), MilitaryBuilding->GetOpponentSpeedZone()->GetMaxReachLevel());

							if (MySpawnInterface)
							{
								MySpawnInterface->SetSpawnVisibility(true);
								MySpawnInterface->SetBuildingLevel(MilitaryBuilding->GetLevel());
								MySpawnInterface->SetColor(Color);
								MySpawnInterface->SetSpawnDetailsEnabled(State->GetAmountOfFood(), State->GetAmountOfCells(), State->GetAmountOfMetal(), State->GetAmountOfCristals());

								for (int i(0); i < SpawnEntityWidgets.Num(); i++)
								{
									SpawnEntityWidgets[i]->SetEntityIsEnabled(MilitaryBuilding->GetLevel(), State->GetAmountOfFood(), State->GetAmountOfCells(), State->GetAmountOfMetal(), State->GetAmountOfCristals());
								}
							}
						}

						if (Building->GetSide() == GetSide())
							LevelInterface->SetAreDetailsVisible(true);
						else
							LevelInterface->SetAreDetailsVisible(false);

						if (Building->GetSide() == Side::Blue)
							LevelInterface->SetLevel(Building->GetLevel(), Building->GetMaxLevel(), Color);
						else if (Building->GetSide() == Side::Red)
							LevelInterface->SetLevel(Building->GetLevel(), Building->GetMaxLevel(), Color);
						else
							LevelInterface->SetLevel(Building->GetLevel(), Building->GetMaxLevel(), Color);

						if (Building->GetLevel() < Building->GetMaxLevel() && !IsAnEconomicPlunderedBuilding)
						{
							LevelInterface->SetIsLevelUpVisible(true);

							int Food = Building->GetCostInFoodToLevel(Building->GetLevel() + 1);
							int Cells = Building->GetCostInCellsToLevel(Building->GetLevel() + 1);
							int Metal = Building->GetCostInMetalToLevel(Building->GetLevel() + 1);
							int Cristals = Building->GetCostInCristalsToLevel(Building->GetLevel() + 1);

							bool IsFoodOk = Food <= State->GetAmountOfFood();
							bool IsCellsOk = Cells <= State->GetAmountOfCells();
							bool IsMetalOk = Metal <= State->GetAmountOfMetal();
							bool IsCristalsOk = Cristals <= State->GetAmountOfCristals();

							LevelInterface->SetRessourcesRequired(Food, Cells, Metal, Cristals, IsFoodOk, IsCellsOk, IsMetalOk, IsCristalsOk);
							LevelInterface->SetIsLevelUpPossible(IsFoodOk && IsCellsOk && IsMetalOk && IsCristalsOk);
						}
						else
							LevelInterface->SetIsLevelUpVisible(false);
					}
				}
			}
			else
			{
				MyStatInterface->SetStatsVisibility(ESlateVisibility::Hidden, ESlateVisibility::Hidden);
				if (MyModesInterface)
					MyModesInterface->SetModesVisibility(ESlateVisibility::Hidden);
				if (MySpawnInterface)
					MySpawnInterface->SetSpawnVisibility(false);
				if (MyEconomicLevelInterface && MyMilitaryLevelInterface)
				{
					MyEconomicLevelInterface->SetIsLevelVisible(false);
					MyMilitaryLevelInterface->SetIsLevelVisible(false);
				}
			}

			MyStatInterface->SetPVs(FinalCurrentLife, FinalMaxLife);
			MyStatInterface->SetHeal(FinalHeal * 2);
			MyStatInterface->SetTheAttack(FinalPhysicAttack, FinalMagicAttack);
			MyStatInterface->SetDefense(FinalPhysicDefense, FinalMagicDefense);
			MyStatInterface->SetSpeed(FinalSpeed);
			MyStatInterface->SetFieldOfSight(FinalFieldOfSight);
			MyStatInterface->SetRange(FinalRange);
			MyStatInterface->SetFoodEaten(FinalFoodEatenInHalfASecond * 2);

			MyStatInterface->SetColor(Color);
		}
	}

	Super::Tick(DeltaTime);
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
	InputComponent->BindAction("Direct", IE_Released, this, &AMousePlayerController::Direct);
	InputComponent->BindAction("AddDirect", IE_Released, this, &AMousePlayerController::AddDirect);

	InputComponent->BindAction("OpponentView", IE_Pressed, this, &AMousePlayerController::EnableOpponentView);
	InputComponent->BindAction("OpponentView", IE_Released, this, &AMousePlayerController::DisableOpponentView);

	InputComponent->BindAxis("MoveForward", this, &AMousePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMousePlayerController::MoveRight);
	InputComponent->BindAxis("Zoom", this, &AMousePlayerController::Zoom);

}

Side AMousePlayerController::GetSide()
{
	return PlayerSide;
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
	if (BoxDisplayed != TypeBox::Target && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
	{
		ClearSelection();
		StartAddSelect();
	}
}
void AMousePlayerController::StartAddSelect()
{
	if (BoxDisplayed != TypeBox::Target && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
	{
		float LocationX;
		float LocationY;
		GetMousePosition(LocationX, LocationY);
		FVector2D StartMousePos;
		StartMousePos.X = LocationX;
		StartMousePos.Y = LocationY;

		HUD->SetStartMousePos(StartMousePos);
		HUD->SetPlayerSide(PlayerSide);
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
	if (BoxDisplayed == TypeBox::Select && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
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
	if (BoxDisplayed != TypeBox::Select && ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()) && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
	{
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->GetSide() == PlayerSide)
			{
				if (PlayerSide == Side::Blue)
				{
					CurrentUnit->ClearDestinations();
					CurrentUnit->ClearSpecialTargets();
				}
				else if (PlayerSide == Side::Red)
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
	if (BoxDisplayed != TypeBox::Select && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
	{
		float LocationX;
		float LocationY;
		GetMousePosition(LocationX, LocationY);
		FVector2D StartMousePos;
		StartMousePos.X = LocationX;
		StartMousePos.Y = LocationY;

		HUD->SetStartMousePos(StartMousePos);
		HUD->SetPlayerSide(PlayerSide);
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
	if (BoxDisplayed == TypeBox::Target && ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()) && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
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
					if (CurrentUnit->GetSide() == PlayerSide && CurrentUnit->GetMode() != Modes::Defense)
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
						if (PlayerSide == Side::Blue)
							Units[k]->AddDestination(HitResult.ImpactPoint + FVector(Position.GetRotated(Rotation.Yaw), 0.f), Rotation);
						else if (PlayerSide == Side::Red)
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

	// Removing doubles and invisible actors
	TArray<TScriptInterface<IGameElementInterface>> TempSelection;
	for (int i(0); i < NewSelection.Num(); i++)
	{
		int index = ActorsSelected.Find(NewSelection[i]);
		if (index == INDEX_NONE && (NewSelection[i]->GetOpponentVisibility() || NewSelection[i]->GetSide() == PlayerSide))
			TempSelection.Add(NewSelection[i]);
	}
	NewSelection = TempSelection;

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
			if (ActorsSelected.IsValidIndex(0) && ActorsSelected.Top().GetObject()->IsA(ABuilding::StaticClass()))
			{
				/*

				if (ActorsSelected.Top().GetObject()->IsA(AMilitaryBuilding::StaticClass()))
				{
				AMilitaryBuilding* MyBuilding = Cast<AMilitaryBuilding>(ActorsSelected.Top().GetObject());

				SetAmountOfFood(50);
				SetAmountOfCells(50);
				SetAmountOfCristals(50);
				SetAmountOfMetal(50);

				SpawnUnit(MyBuilding, AKnight::StaticClass());
				}

				*/

				ActorsSelected[0]->Unselect();
				ActorsSelected.RemoveAt(0);
			}
			else if (FinalSelection.IsValidIndex(0) && Cast<AActor>(FinalSelection.Top().GetObject())->IsA(ABuilding::StaticClass()))
				FinalSelection.RemoveAt(0);

			//Si la nouvelle unité sélectionnée est alliée
			if (NewSelection[i]->GetSide() == PlayerSide)
			{
				// Si le premier acteur sélectionné est une unité ennemie, on le supprime de la sélection
				if (ActorsSelected.IsValidIndex(0) && ActorsSelected.Top()->GetSide() != PlayerSide)
				{
					ActorsSelected[0]->Unselect();
					ActorsSelected.RemoveAt(0);
				}
				else if (FinalSelection.IsValidIndex(0) && FinalSelection.Top()->GetSide() != PlayerSide)
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
				if (AllSelectedBefore[0] == NewSelection[i])
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
			if (CurrentUnit->GetSide() == PlayerSide)
			{
				if (Role == ROLE_Authority)
					CurrentUnit->SetBoxSpecialTargets(NewTargets);
				else
				{
					TArray<AActor*> UpdatedNewTargets;
					for (int j(0); j < NewTargets.Num(); j++)
						UpdatedNewTargets.Add(Cast<AActor>(NewTargets[j].GetObject()));

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

//EnemyView
void AMousePlayerController::SetOpponentView(bool Enable)
{
	OpponentView = Enable;

	TActorIterator<AMilitaryBuilding> BuildingItr(GetWorld());
	for (BuildingItr; BuildingItr; ++BuildingItr)
	{
		BuildingItr->ChangeDecals();
	}

	FogOfWar();
}
bool AMousePlayerController::IsOpponentViewEnabled()
{
	return OpponentView;
}
void AMousePlayerController::EnableOpponentView()
{
  	SetOpponentView(true);
}
void AMousePlayerController::DisableOpponentView()
{
	SetOpponentView(false);
}

//Fog Of War
void AMousePlayerController::FogOfWar()
{
	TArray<TScriptInterface<IGameElementInterface>> BlueActors;
	TArray<TScriptInterface<IGameElementInterface>> RedActors;
	TArray<TScriptInterface<IGameElementInterface>> NeutralActors;

	TArray<AMilitaryBuilding*> MilitaryBuildings;
	TArray<AUnit*> Units;

	TActorIterator<AUnit> UnitItr(GetWorld());
	for (UnitItr; UnitItr; ++UnitItr)
	{
		if (UnitItr->GetSide() == Side::Blue)
			BlueActors.Add(TScriptInterface<IGameElementInterface>(*UnitItr));
		else if (UnitItr->GetSide() == Side::Red)
			RedActors.Add(TScriptInterface<IGameElementInterface>(*UnitItr));
		else
			NeutralActors.Add(TScriptInterface<IGameElementInterface>(*UnitItr));
		Units.Add(*UnitItr);
	}

	TActorIterator<ABuilding> BuildingItr(GetWorld());
	for (BuildingItr; BuildingItr; ++BuildingItr)
	{
		if (BuildingItr->GetSide() == Side::Blue)
			BlueActors.Add(TScriptInterface<IGameElementInterface>(*BuildingItr));
		else if (BuildingItr->GetSide() == Side::Red)
			RedActors.Add(TScriptInterface<IGameElementInterface>(*BuildingItr));
		else
			NeutralActors.Add(TScriptInterface<IGameElementInterface>(*BuildingItr));

		if (BuildingItr->IsA(AMilitaryBuilding::StaticClass()))
			MilitaryBuildings.Add(Cast<AMilitaryBuilding>(*BuildingItr));
	}


	bool ShouldBeVisible(false);
	if (Role == ROLE_Authority)
	{
		ApplyZoneEffects(MilitaryBuildings, Units);

		for (int i = 0; i < BlueActors.Num(); i++)
		{
			if (BlueActors[i].GetObject()->IsA(AUnit::StaticClass()))
			{
				ShouldBeVisible = false;
				TArray<TScriptInterface<IGameElementInterface>> OpponentsInSight;
				AUnit* BlueUnit = Cast<AUnit>(BlueActors[i].GetObject());

				for (int j = 0; j < RedActors.Num(); j++)
				{
					bool IsRedActorInvisible(false);
					if (RedActors[j].GetObject()->IsA(AUnit::StaticClass()))
						IsRedActorInvisible = Cast<AUnit>(RedActors[j].GetObject())->GetMode() == Modes::Invisible;

					if (FVector(RedActors[j]->GetLocation() - BlueActors[i]->GetLocation()).Size() <= RedActors[j]->GetFieldOfSight() * 100.f && BlueUnit->GetMode() != Modes::Invisible)
						ShouldBeVisible = true;
					if (FVector(RedActors[j]->GetLocation() - BlueActors[i]->GetLocation()).Size() <= BlueActors[i]->GetFieldOfSight() * 100.f && !IsRedActorInvisible)
						OpponentsInSight.Add(RedActors[j]);
				}
				for (int j = 0; j < NeutralActors.Num(); j++)
				{
					if (FVector(NeutralActors[j]->GetLocation() - BlueActors[i]->GetLocation()).Size() <= BlueActors[i]->GetFieldOfSight() * 100.f && BlueUnit->GetMode() != Modes::Invisible)
						OpponentsInSight.Add(NeutralActors[j]);
				}

				if (BlueUnit->GetOpponentVisibility() != ShouldBeVisible)
					BlueUnit->Multicast_SetVisibility(ShouldBeVisible, BlueUnit->GetLocation(), BlueUnit->GetActorRotation());
				BlueUnit->SetOpponentsInSight(OpponentsInSight);
			}
		}
		for (int i = 0; i < RedActors.Num(); i++)
		{
			if (RedActors[i].GetObject()->IsA(AUnit::StaticClass()))
			{
				ShouldBeVisible = false;
				TArray<TScriptInterface<IGameElementInterface>> OpponentsInSight;
				AUnit* RedUnit = Cast<AUnit>(RedActors[i].GetObject());

				for (int j = 0; j < BlueActors.Num(); j++)
				{
					bool IsBlueActorInvisible(false);
					if (BlueActors[j].GetObject()->IsA(AUnit::StaticClass()))
						IsBlueActorInvisible = Cast<AUnit>(BlueActors[j].GetObject())->GetMode() == Modes::Invisible;

					if (FVector(BlueActors[j]->GetLocation() - RedActors[i]->GetLocation()).Size() <= BlueActors[j]->GetFieldOfSight() * 100.f && RedUnit->GetMode() != Modes::Invisible)
						ShouldBeVisible = true;
					if (FVector(BlueActors[j]->GetLocation() - RedActors[i]->GetLocation()).Size() <= RedActors[i]->GetFieldOfSight() * 100.f && !IsBlueActorInvisible)
						OpponentsInSight.Add(BlueActors[j]);
				}
				for (int j = 0; j < NeutralActors.Num(); j++)
				{
					if (FVector(NeutralActors[j]->GetLocation() - RedActors[i]->GetLocation()).Size() <= RedActors[i]->GetFieldOfSight() * 100.f && RedUnit->GetMode() != Modes::Invisible)
						OpponentsInSight.Add(NeutralActors[j]);
				}

				if (RedUnit->GetOpponentVisibility() != ShouldBeVisible)
					RedUnit->Multicast_SetVisibility(ShouldBeVisible, RedUnit->GetLocation(), RedUnit->GetActorRotation());
				RedUnit->SetOpponentsInSight(OpponentsInSight);
			}
		}
	}

	if (!OpponentView)
	{
		if (PlayerSide == Side::Blue)
			SetFogOfWarTexture(BlueActors);
		else
			SetFogOfWarTexture(RedActors);
	}
	else
	{
		if (PlayerSide == Side::Blue)
		{
			TArray<TScriptInterface<IGameElementInterface>> VisibleRedActors;
			for (int i = 0; i < RedActors.Num(); i++)
			{
				if (RedActors[i]->GetOpponentVisibility())
					VisibleRedActors.Add(RedActors[i]);
			}
			SetFogOfWarTexture(VisibleRedActors);
		}
		else
		{
			TArray<TScriptInterface<IGameElementInterface>> VisibleBlueActors;
			for (int i = 0; i < BlueActors.Num(); i++)
			{
				if (BlueActors[i]->GetOpponentVisibility())
					VisibleBlueActors.Add(BlueActors[i]);
			}
			SetFogOfWarTexture(VisibleBlueActors);
		}
	}
}
void AMousePlayerController::SetFogOfWarTexture(TArray<TScriptInterface<IGameElementInterface>> Actors)
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

		for (int i(0); i < Actors.Num(); i++)
		{
			FVector TempPosition = Actors[i]->GetLocation();
			UnitsPosition.Add(FVector(TempPosition.X, TempPosition.Y, 0.f));
			UnitsSight.Add(Actors[i]->GetFieldOfSight());
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
void AMousePlayerController::ApplyZoneEffects(TArray<AMilitaryBuilding*> MilitaryBuildings, TArray<AUnit*> Units)
{
	if (Role == ROLE_Authority && GetWorld()->GetAuthGameMode() && GetWorld()->GetGameState<AMultiplayerGameState>()->DidGameBegin())
	{
		for (int j(0); j < Units.Num(); j++)
		{
			float Multiplicator = 1.f;
			int Heal = 0;
			for (int i(0); i < MilitaryBuildings.Num(); i++)
			{
				if (MilitaryBuildings[i]->GetSide() == Units[j]->GetSide())
				{
					Heal += MilitaryBuildings[i]->GetPlayerLifeZone()->GetLifeModifier(Units[j], MilitaryBuildings[i]);
					Multiplicator *= MilitaryBuildings[i]->GetPlayerSpeedZone()->GetSpeedMultiplicator(Units[j], MilitaryBuildings[i]);
				}
				else
				{
					Heal += MilitaryBuildings[i]->GetOpponentLifeZone()->GetLifeModifier(Units[j], MilitaryBuildings[i]);
					Multiplicator *= MilitaryBuildings[i]->GetOpponentSpeedZone()->GetSpeedMultiplicator(Units[j], MilitaryBuildings[i]);
				}
			}
			Units[j]->SetSpeedMultiplicator(Multiplicator);
			Units[j]->Heal(Heal * 0.1);
		}
	}
}

void AMousePlayerController::Server_AddDestination_Implementation(AUnit *Unit, FVector Destination, FRotator Rotation)
{
	Unit->AddDestination(Destination, Rotation);
}
bool AMousePlayerController::Server_AddDestination_Validate(AUnit *Unit, FVector Destination, FRotator Rotation)
{
	return Unit != NULL && Unit->GetSide() == Side::Red && !Unit->IsPendingKill();
}
void AMousePlayerController::Server_ClearDestinations_Implementation(AUnit *Unit)
{
	Unit->ClearDestinations();
}
bool AMousePlayerController::Server_ClearDestinations_Validate(AUnit *Unit)
{
	return Unit != NULL && Unit->GetSide() == Side::Red && !Unit->IsPendingKill();
}

void AMousePlayerController::Server_AddSpecialTargets_Implementation(AUnit *Unit)
{
	Unit->AddSpecialTargets();
}
bool AMousePlayerController::Server_AddSpecialTargets_Validate(AUnit *Unit)
{
	return Unit != NULL && Unit->GetSide() == Side::Red && !Unit->IsPendingKill();
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
	return Unit != NULL && Unit->GetSide() == Side::Red && !Unit->IsPendingKill();
}
void AMousePlayerController::Server_ClearSpecialTargets_Implementation(AUnit *Unit)
{
	Unit->ClearSpecialTargets();
}
bool AMousePlayerController::Server_ClearSpecialTargets_Validate(AUnit *Unit)
{
	return Unit != NULL && Unit->GetSide() == Side::Red && !Unit->IsPendingKill();
}

//Costing functions
void AMousePlayerController::Server_LevelUpBuilding_Implementation(ABuilding* Building)
{
	if (Role == ROLE_Authority && Building->GetLevel() < Building->GetMaxLevel())
	{
		int CostInCells = Building->GetCostInCellsToLevel(Building->GetLevel() + 1);
		int CostInMetal = Building->GetCostInMetalToLevel(Building->GetLevel() + 1);
		int CostInFood = Building->GetCostInFoodToLevel(Building->GetLevel() + 1);
		int CostInCristals = Building->GetCostInCristalsToLevel(Building->GetLevel() + 1);

		AMultiplayerState* State = Cast<AMultiplayerState>(PlayerState);

		if (State->GetAmountOfCells() >= CostInCells && State->GetAmountOfMetal() >= CostInMetal && State->GetAmountOfFood() >= CostInFood && State->GetAmountOfCristals() >= CostInCristals)
		{
			Building->LevelUp();

			State->SetAmountOfCells(State->GetAmountOfCells() - CostInCells);
			State->SetAmountOfMetal(State->GetAmountOfMetal() - CostInMetal);
			State->SetAmountOfFood(State->GetAmountOfFood() - CostInFood);
			State->SetAmountOfCristals(State->GetAmountOfCristals() - CostInCristals);
		}
	}
}
bool AMousePlayerController::Server_LevelUpBuilding_Validate(ABuilding* Building)
{
	return true;
}
void AMousePlayerController::Server_SpawnUnit_Implementation(AMilitaryBuilding* Spawner, TSubclassOf<AUnit> Unit)
{
	if (Role == ROLE_Authority && Spawner->GetSide() == PlayerSide && Spawner->GetLevel() >= Cast<AUnit>(Unit->GetDefaultObject())->GetBuildingLevelRequired())
	{
		int CostInCells = Cast<AUnit>(Unit->GetDefaultObject())->GetCostInCells();
		int CostInMetal = Cast<AUnit>(Unit->GetDefaultObject())->GetCostInMetal();
		int CostInFood = Cast<AUnit>(Unit->GetDefaultObject())->GetCostInFood();
		int CostInCristals = Cast<AUnit>(Unit->GetDefaultObject())->GetCostInCristals();

		AMultiplayerState* State = Cast<AMultiplayerState>(PlayerState);

		if (State->GetAmountOfCells() >= CostInCells && State->GetAmountOfMetal() >= CostInMetal && State->GetAmountOfFood() >= CostInFood && State->GetAmountOfCristals() >= CostInCristals)
		{
			FNavLocation Location;

			if (GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(Spawner->GetLocation(), Location, FVector(Spawner->GetSize() + 50.f, Spawner->GetSize() + 50.f, 500.f)))
			{
				FVector Position(Location.Location.X, Location.Location.Y, Location.Location.Z + Cast<AUnit>(Unit->GetDefaultObject())->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
				FRotator Rotation(0.f, 0.f, 0.f);

				AUnit* NewUnit = GetWorld()->SpawnActor<AUnit>(Unit, Position, Rotation);
				if (NewUnit)
				{
					NewUnit->SetSide(PlayerSide);

					State->SetAmountOfCells(State->GetAmountOfCells() - CostInCells);
					State->SetAmountOfMetal(State->GetAmountOfMetal() - CostInMetal);
					State->SetAmountOfFood(State->GetAmountOfFood() - CostInFood);
					State->SetAmountOfCristals(State->GetAmountOfCristals() - CostInCristals);
				}
			}
		}
	}
}
bool AMousePlayerController::Server_SpawnUnit_Validate(AMilitaryBuilding* Spawner, TSubclassOf<AUnit> Unit)
{
	return true;
}

//Modes
void AMousePlayerController::SetMode(Modes Mode)
{
	if (ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
	{
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* Unit = Cast<AUnit>(ActorsSelected[i].GetObject());

			Server_ChangeMode(Unit, Mode);
		}
	}
}
void AMousePlayerController::Server_ChangeMode_Implementation(AUnit* Unit, Modes Mode)
{
	Unit->ChangeMode(Mode);
}
bool AMousePlayerController::Server_ChangeMode_Validate(AUnit* Unit, Modes Mode)
{
	return true;
}

void AMousePlayerController::SetModeToAttack()
{
	SetMode(Modes::Attack);
}
void AMousePlayerController::SetModeToDefense()
{
	SetMode(Modes::Defense);
}
void AMousePlayerController::SetModeToSpeed()
{
	SetMode(Modes::Movement);
}
void AMousePlayerController::SetModeToSight()
{
	SetMode(Modes::Alert);
}
void AMousePlayerController::SetModeToInvisible()
{
	SetMode(Modes::Invisible);
}

//LevelUp
void AMousePlayerController::LevelUp()
{
	if (ActorsSelected[0].GetObject()->IsA(ABuilding::StaticClass()))
	{
		ABuilding* Building = Cast<ABuilding>(ActorsSelected[0].GetObject());

		Server_LevelUpBuilding(Building);
	}
}
void AMousePlayerController::Server_LevelUpZone_Implementation(AMilitaryBuilding* MilitaryBuilding, bool IsPlayer, bool IsLife, bool IsEffect)
{
	if (MilitaryBuilding->GetPoints() > 0)
	{
		if (IsPlayer)
		{
			if (IsLife)
			{
				if (IsEffect)
				{
					if (MilitaryBuilding->GetPlayerLifeZone()->GetCurrentEffectLevel() < MilitaryBuilding->GetPlayerLifeZone()->GetMaxEffectLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpPlayerLifeZoneEffect();
					}
				}
				else
				{
					if (MilitaryBuilding->GetPlayerLifeZone()->GetCurrentReachLevel() < MilitaryBuilding->GetPlayerLifeZone()->GetMaxReachLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpPlayerLifeZoneReach();
					}
				}
			}
			else
			{
				if (IsEffect)
				{
					if (MilitaryBuilding->GetPlayerSpeedZone()->GetCurrentEffectLevel() < MilitaryBuilding->GetPlayerSpeedZone()->GetMaxEffectLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpPlayerSpeedZoneEffect();
					}
				}
				else
				{
					if (MilitaryBuilding->GetPlayerSpeedZone()->GetCurrentReachLevel() < MilitaryBuilding->GetPlayerSpeedZone()->GetMaxReachLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpPlayerSpeedZoneReach();
					}
				}
			}
		}
		else
		{
			if (IsLife)
			{
				if (IsEffect)
				{
					if (MilitaryBuilding->GetOpponentLifeZone()->GetCurrentEffectLevel() < MilitaryBuilding->GetOpponentLifeZone()->GetMaxEffectLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpOpponentLifeZoneEffect();
					}
				}
				else
				{
					if (MilitaryBuilding->GetOpponentLifeZone()->GetCurrentReachLevel() < MilitaryBuilding->GetOpponentLifeZone()->GetMaxReachLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpOpponentLifeZoneReach();
					}
				}
			}
			else
			{
				if (IsEffect)
				{
					if (MilitaryBuilding->GetOpponentSpeedZone()->GetCurrentEffectLevel() < MilitaryBuilding->GetOpponentSpeedZone()->GetMaxEffectLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpOpponentSpeedZoneEffect();
					}
				}
				else
				{
					if (MilitaryBuilding->GetOpponentSpeedZone()->GetCurrentReachLevel() < MilitaryBuilding->GetOpponentSpeedZone()->GetMaxReachLevel())
					{
						MilitaryBuilding->UseOnePoint();
						MilitaryBuilding->Multicast_LevelUpOpponentSpeedZoneReach();
					}
				}
			}
		}
	}
}
bool AMousePlayerController::Server_LevelUpZone_Validate(AMilitaryBuilding* MilitaryBuilding, bool IsPlayer, bool IsLife, bool IsEffect)
{
	if (MilitaryBuilding)
		return true;
	else
		return false;
}
void AMousePlayerController::LevelUpZone(bool IsPlayer, bool IsLife, bool IsEffect)
{
	if (ActorsSelected[0] && ActorsSelected[0].GetObject()->IsA(AMilitaryBuilding::StaticClass()))
	{
		AMilitaryBuilding* MilitaryBuilding = Cast<AMilitaryBuilding>(ActorsSelected[0].GetObject());

		Server_LevelUpZone(MilitaryBuilding, IsPlayer, IsLife, IsEffect);
	}
}

//Spawn
void AMousePlayerController::SpawnUnit(TSubclassOf<AUnit> Unit)
{
	if (ActorsSelected[0].GetObject()->IsA(AMilitaryBuilding::StaticClass()))
	{
		AMilitaryBuilding* Building = Cast<AMilitaryBuilding>(ActorsSelected[0].GetObject());

		Server_SpawnUnit(Building, Unit);
	}
}
USpawnEntityWidget* AMousePlayerController::PrepareEntity(TSubclassOf<AUnit> UnitClass)
{
	USpawnEntityWidget* SpawnEntity = CreateWidget<USpawnEntityWidget>(this, wSpawnEntityInterface);
	AUnit* UnitDefaultObject = Cast<AUnit>(UnitClass->GetDefaultObject());

	SpawnEntity->SetUnit(UnitClass, UnitDefaultObject->GetName(), UnitDefaultObject->GetUnitImage());
	SpawnEntity->SetRessourcesRequired(UnitDefaultObject->GetCostInFood(), UnitDefaultObject->GetCostInCells(), UnitDefaultObject->GetCostInMetal(), UnitDefaultObject->GetCostInCristals());

	SpawnEntity->SetLevelRequired(UnitDefaultObject->GetBuildingLevelRequired());
	SpawnEntity->SetPVs(UnitDefaultObject->GetMaxLife());
	SpawnEntity->SetTheAttack(UnitDefaultObject->GetPhysicAttack(), UnitDefaultObject->GetMagicAttack());
	SpawnEntity->SetDefense(UnitDefaultObject->GetPhysicDefense(), UnitDefaultObject->GetMagicDefense());
	SpawnEntity->SetSpeed(UnitDefaultObject->GetSpeed());
	SpawnEntity->SetFieldOfSight(UnitDefaultObject->GetFieldOfSight());
	SpawnEntity->SetRange(UnitDefaultObject->GetRange());
	SpawnEntity->SetFoodEaten(UnitDefaultObject->GetFoodEatenInHalfASecond());

	return SpawnEntity;
}

//Replication
void AMousePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AMousePlayerController, PlayerSide);
	DOREPLIFETIME(AMousePlayerController, PlayerState);
}