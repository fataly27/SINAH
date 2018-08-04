// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Units/Unit.h"
#include "Buildings/Building.h"
#include "MultiplayerGameState.h"
#include <cmath>
#include "PlayerHUD.h"

FMyPlane::FMyPlane() : FMyPlane(FVector(0.f), FVector(0.f), FVector(0.f)) {}
FMyPlane::FMyPlane(FVector A, FVector B, FVector C) : Super(A, B, C) {}

void FMyPlane::FixX(float x)
{
	W -= x * X;
	X = 0;
}
void FMyPlane::FixY(float y)
{
	W -= y * Y;
	Y = 0;
}
void FMyPlane::FixZ(float z)
{
	W -= z * Z;
	Z = 0;
}

float FMyPlane::GetXValue()
{
	return W / X;
}
float FMyPlane::GetYValue()
{
	return W / Y;
}
float FMyPlane::GetZValue()
{
	return W / Z;
}

// -------------------------------------

APlayerHUD::APlayerHUD() : Super(), StartMousePos(0.f, 0.f), CurrentMousePos(0.f, 0.f), BoxDisplayed(ETypeBox::None)
{
	static ConstructorHelpers::FObjectFinder<UTexture> TheBlueDestination(TEXT("Texture'/Game/Textures/BlueDestination.BlueDestination'"));
	static ConstructorHelpers::FObjectFinder<UTexture> TheRedDestination(TEXT("Texture'/Game/Textures/RedDestination.RedDestination'"));
	static ConstructorHelpers::FObjectFinder<UTexture> TheBlueTarget(TEXT("Texture'/Game/Textures/blue_target.blue_target'"));
	static ConstructorHelpers::FObjectFinder<UTexture> TheRedTarget(TEXT("Texture'/Game/Textures/red_target.red_target'"));
	static ConstructorHelpers::FObjectFinder<UTexture> TheBlueTargetLight(TEXT("Texture'/Game/Textures/blue_target_light.blue_target_light'"));
	static ConstructorHelpers::FObjectFinder<UTexture> TheRedTargetLight(TEXT("Texture'/Game/Textures/red_target_light.red_target_light'"));

	BlueDestinationTexture = TheBlueDestination.Object;
	RedDestinationTexture = TheRedDestination.Object;
	BlueTargetTexture = TheBlueTarget.Object;
	RedTargetTexture = TheRedTarget.Object;
	BlueTargetTextureLight = TheBlueTargetLight.Object;
	RedTargetTextureLight = TheRedTargetLight.Object;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	SetPlayerSide(ESide::Blue);
}

void APlayerHUD::DrawHUD()
{
	if (BoxDisplayed == ETypeBox::Select)
	{
		if (PlayerSide == ESide::Blue)
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(80, 80, 255));
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(80, 80, 255));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(80, 80, 255));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(80, 80, 255));

			DrawRect(FLinearColor(0, 0, 1, 0.15f), StartMousePos.X, StartMousePos.Y, CurrentMousePos.X - StartMousePos.X, CurrentMousePos.Y - StartMousePos.Y);
		}
		else if (PlayerSide == ESide::Red)
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 80, 80));
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 80, 80));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 80, 80));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 80, 80));

			DrawRect(FLinearColor(1, 0, 0, 0.15f), StartMousePos.X, StartMousePos.Y, CurrentMousePos.X - StartMousePos.X, CurrentMousePos.Y - StartMousePos.Y);
		}

		TArray<AUnit*> Units;
		TArray<ABuilding*> Buildings;
		Units = GetUnitsInRectangle(StartMousePos, CurrentMousePos);
		Buildings = GetBuildingsInRectangle(StartMousePos, CurrentMousePos);

		ActorsBeingSelected.Empty();
		for (int i = 0; i < Units.Num(); i++)
			ActorsBeingSelected.Add(TScriptInterface<IGameElementInterface>(Units[i]));
		for (int i = 0; i < Buildings.Num(); i++)
			ActorsBeingSelected.AddUnique(TScriptInterface<IGameElementInterface>(Buildings[i]));
	}
	else if (BoxDisplayed == ETypeBox::Target)
	{
		if (PlayerSide == ESide::Blue)
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(150, 150, 255), 2.f);
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(150, 150, 255), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(150, 150, 255), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(150, 150, 255), 2.f);
		}
		else if (PlayerSide == ESide::Red)
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 150, 150), 2.f);
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 150, 150), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 150, 150), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 150, 150), 2.f);
		}

		TArray<AUnit*> Units;
		TArray<ABuilding*> Buildings;
		Units = GetUnitsInRectangle(StartMousePos, CurrentMousePos);
		Buildings = GetBuildingsInRectangle(StartMousePos, CurrentMousePos);

		ActorsBeingSelected.Empty();
		for (int i = 0; i < Units.Num(); i++)
			ActorsBeingSelected.Add(TScriptInterface<IGameElementInterface>(Units[i]));
		for (int i = 0; i < Buildings.Num(); i++)
			ActorsBeingSelected.AddUnique(TScriptInterface<IGameElementInterface>(Buildings[i]));
	}

	for (int i(0); i < ActorsSelected.Num(); i++)
	{
		if (ActorsSelected[i]->IsSelected())
		{
			FVector2D TopPosition(Project(ActorsSelected[i]->GetLocation() + FVector(0.f, 0.f, ActorsSelected[i]->GetHalfHeight())));
			float Percent = float(ActorsSelected[i]->GetCurrentLife()) / float(ActorsSelected[i]->GetMaxLife());
			float HeightOfRects = FMath::RoundHalfFromZero(50.f / CurrentZoom) + 1.f;
			float WidthOfRect = ActorsSelected[i]->GetLifeBarWidth() / CurrentZoom;
			float Offset = 100.f / CurrentZoom + 2.f;

			if (ActorsSelected[i]->GetSide() == ESide::Blue)
			{
				DrawRect(FLinearColor(0, 0, 1, 0.25f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect, HeightOfRects);
				DrawRect(FLinearColor(0, 0, 1, 0.75f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect * Percent, HeightOfRects);
			}
			else if (ActorsSelected[i]->GetSide() == ESide::Red)
			{
				DrawRect(FLinearColor(1, 0, 0, 0.25f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect, HeightOfRects);
				DrawRect(FLinearColor(1, 0, 0, 0.75f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect * Percent, HeightOfRects);
			}
			else
			{
				DrawRect(FLinearColor(0.5f, 0.5f, 0.5f, 0.25f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect, HeightOfRects);
				DrawRect(FLinearColor(0.5f, 0.5f, 0.5f, 0.75f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect * Percent, HeightOfRects);
			}
		}
	}

	if (ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
	{
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->GetSide() == PlayerSide && CurrentUnit->GetMode() != EModes::Defense)
			{
				TArray<FVector> Destinations = CurrentUnit->GetDestinations();
				for (int j(0); j < Destinations.Num(); j++)
				{
					FVector2D PositionOnScreen(Project(Destinations[j]));
					if (PlayerSide == ESide::Blue)
						DrawTextureSimple(BlueDestinationTexture, PositionOnScreen.X - BlueDestinationTexture->GetSurfaceWidth() / 2, PositionOnScreen.Y - BlueDestinationTexture->GetSurfaceHeight() / 2);
					else if (PlayerSide == ESide::Red)
						DrawTextureSimple(RedDestinationTexture, PositionOnScreen.X - RedDestinationTexture->GetSurfaceWidth() / 2, PositionOnScreen.Y - RedDestinationTexture->GetSurfaceHeight() / 2);
				}
			}
		}

		TArray<TScriptInterface<IGameElementInterface>> AllTargets;
		TArray<int> NumberOfOccurences;
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->GetSide() == PlayerSide)
			{
				TArray<TScriptInterface<IGameElementInterface>> TargetsOfActor = CurrentUnit->GetSpecialTargets();
				for (int j(0); j < TargetsOfActor.Num(); j++)
				{
					int index = AllTargets.AddUnique(TargetsOfActor[j]);
					if (NumberOfOccurences.Num() > index)
						NumberOfOccurences[index]++;
					else
						NumberOfOccurences.Add(1);
				}
			}
		}
		for (int i(0); i < AllTargets.Num(); i++)
		{
			FVector2D TopPosition(Project(AllTargets[i]->GetLocation() + FVector(0.f, 0.f, AllTargets[i]->GetHalfHeight())));
			float Scale = 5.f / CurrentZoom + 0.2f;
			float Offset = 100.f / CurrentZoom + 2.f;
			if (NumberOfOccurences[i] == ActorsSelected.Num())
			{
				if (PlayerSide == ESide::Blue)
					DrawTextureSimple(BlueTargetTexture, TopPosition.X - BlueTargetTexture->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - BlueTargetTexture->GetSurfaceHeight() * Scale / 2, Scale);
				else if (PlayerSide == ESide::Red)
					DrawTextureSimple(RedTargetTexture, TopPosition.X - RedTargetTexture->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - RedTargetTexture->GetSurfaceHeight() * Scale / 2, Scale);
			}
			else
			{
				if (PlayerSide == ESide::Blue)
					DrawTextureSimple(BlueTargetTextureLight, TopPosition.X - BlueTargetTextureLight->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - BlueTargetTextureLight->GetSurfaceHeight() * Scale / 2, Scale);
				else if (PlayerSide == ESide::Red)
					DrawTextureSimple(RedTargetTextureLight, TopPosition.X - RedTargetTextureLight->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - RedTargetTextureLight->GetSurfaceHeight() * Scale / 2, Scale);
			}
		}
	}
}

void APlayerHUD::SetStartMousePos(FVector2D Pos)
{
	StartMousePos = Pos;
}
void APlayerHUD::SetCurrentMousePos(FVector2D Pos)
{
	CurrentMousePos = Pos;
}
void APlayerHUD::ShouldDisplayBox(ETypeBox Display)
{
	BoxDisplayed = Display;
}
void APlayerHUD::SetPlayerSide(ESide NewSide)
{
	PlayerSide = NewSide;
}
bool APlayerHUD::IsBoxTiny()
{
	return abs(StartMousePos.X - CurrentMousePos.X) < 6 && abs(StartMousePos.Y - CurrentMousePos.Y) < 6;
}

TArray<TScriptInterface<IGameElementInterface>> APlayerHUD::GetActorsBeingSelected()
{
	TArray<TScriptInterface<IGameElementInterface>> ArrayToSend(ActorsBeingSelected);
	ActorsBeingSelected.Empty();
	return ArrayToSend;
}
void APlayerHUD::SetActorsSelected(TArray<TScriptInterface<IGameElementInterface>> Actors)
{
	ActorsSelected = Actors;
}
void APlayerHUD::SetZoom(float Zoom)
{
	CurrentZoom = Zoom;
}
bool APlayerHUD::GetIsInSelection(FVector2D TempStart, FVector2D TempEnd, FVector ActorLocation, FVector ActorBoxExtent)
{
	FVector2D Start, End;
	if (TempStart.X <= TempEnd.X)
	{
		Start.X = TempStart.X;
		End.X = TempEnd.X;
	}
	else
	{
		Start.X = TempEnd.X;
		End.X = TempStart.X;
	}
	if (TempStart.Y <= TempEnd.Y)
	{
		Start.Y = TempStart.Y;
		End.Y = TempEnd.Y;
	}
	else
	{
		Start.Y = TempEnd.Y;
		End.Y = TempStart.Y;
	}

	TArray<FVector, TInlineAllocator <8>> Vertices;

	FVector Location, Direction;

	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(Start.X, Start.Y, Location, Direction);
	Vertices.Add(Location + Direction * 100.f);
	Vertices.Add(Location - Direction * (Location.Z / Direction.Z));
	//
	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(Start.X, End.Y, Location, Direction);
	Vertices.Add(Location + Direction * 100.f);
	Vertices.Add(Location - Direction * (Location.Z / Direction.Z));
	//
	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(End.X, Start.Y, Location, Direction);
	Vertices.Add(Location + Direction * 100.f);
	Vertices.Add(Location - Direction * (Location.Z / Direction.Z));
	//
	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(End.X, End.Y, Location, Direction);
	Vertices.Add(Location + Direction * 100.f);
	Vertices.Add(Location - Direction * (Location.Z / Direction.Z));
	
	int SideModifier = 1;
	if (PlayerSide == ESide::Red)
		SideModifier = -1;

	TArray<FMyPlane, TInlineAllocator <4>> Planes;
	Planes.Add(FMyPlane(Vertices[0], Vertices[1], Vertices[2]));
	Planes[0].W += (-SideModifier * ActorBoxExtent.X * Planes[0].X - SideModifier * ActorBoxExtent.Y * Planes[0].Y + ActorBoxExtent.Z * Planes[0].Z) / 2.f;
	Planes.Add(FMyPlane(Vertices[0], Vertices[1], Vertices[4]));
	Planes[1].W += (SideModifier * ActorBoxExtent.X * Planes[1].X - SideModifier * ActorBoxExtent.Y * Planes[1].Y + ActorBoxExtent.Z * Planes[1].Z) / 2.f;
	Planes.Add(FMyPlane(Vertices[4], Vertices[6], Vertices[7]));
	Planes[2].W += (SideModifier * ActorBoxExtent.X * Planes[2].X + SideModifier * ActorBoxExtent.Y * Planes[2].Y + ActorBoxExtent.Z * Planes[2].Z) / 2.f;
	Planes.Add(FMyPlane(Vertices[2], Vertices[6], Vertices[7]));
	Planes[3].W += (-SideModifier  * ActorBoxExtent.X * Planes[3].X + SideModifier * ActorBoxExtent.Y * Planes[3].Y - ActorBoxExtent.Z * Planes[3].Z) / 2.f;

	for (int i(0); i < Planes.Num(); i++)
	{
		Planes[i].FixZ(ActorLocation.Z);
	}

	FMyPlane One = Planes[0];
	FMyPlane Two = Planes[2];
	One.FixX(ActorLocation.X);
	Two.FixX(ActorLocation.X);
	if (FMath::Min(One.GetYValue(), Two.GetYValue()) <= ActorLocation.Y && FMath::Max(One.GetYValue(), Two.GetYValue()) >= ActorLocation.Y)
	{
		One = Planes[0];
		Two = Planes[2];
		One.FixY(ActorLocation.Y);
		Two.FixY(ActorLocation.Y);
		if (FMath::Min(One.GetXValue(), Two.GetXValue()) <= ActorLocation.X && FMath::Max(One.GetXValue(), Two.GetXValue()) >= ActorLocation.X)
		{
			One = Planes[1];
			Two = Planes[3];
			One.FixX(ActorLocation.X);
			Two.FixX(ActorLocation.X);
			if (FMath::Min(One.GetYValue(), Two.GetYValue()) <= ActorLocation.Y && FMath::Max(One.GetYValue(), Two.GetYValue()) >= ActorLocation.Y)
			{
				One = Planes[1];
				Two = Planes[3];
				One.FixY(ActorLocation.Y);
				Two.FixY(ActorLocation.Y);
				if (FMath::Min(One.GetXValue(), Two.GetXValue()) <= ActorLocation.X && FMath::Max(One.GetXValue(), Two.GetXValue()) >= ActorLocation.X)
					return true;
			}
		}
	}
	return false;
}
AActor* APlayerHUD::GetSingleSelection(FVector2D ScreenPosition)
{
	FVector Location, Direction;

	GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, Location, Direction);
	FVector StartLocation = Location + Direction * 100.f;
	FVector EndLocation = Location - Direction * (Location.Z / Direction.Z);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Pawn))
		return HitResult.GetActor();
	else
		return nullptr;
}
TArray<AUnit*> APlayerHUD::GetUnitsInRectangle(FVector2D Start, FVector2D End)
{
	TArray<AUnit*> Actors;

	if (Start != End)
	{
		TActorIterator<AUnit> ActorsItr(GetWorld());
		FVector BoxOrigin, BoxExtent;
		for (ActorsItr; ActorsItr; ++ActorsItr)
		{
			ActorsItr->GetActorBounds(false, BoxOrigin, BoxExtent);

			if (GetIsInSelection(Start, End, BoxOrigin, BoxExtent))
				Actors.Add(*ActorsItr);
		}
	}
	else
	{
		AActor* Actor = GetSingleSelection(Start);
		if(IsValid(Actor) && Actor->IsA(AUnit::StaticClass()))
			Actors.Add(Cast<AUnit>(Actor));
	}

	return Actors;
}
TArray<ABuilding*> APlayerHUD::GetBuildingsInRectangle(FVector2D Start, FVector2D End)
{
	TArray<ABuilding*> Actors;

	if (Start != End)
	{
		TActorIterator<ABuilding> ActorsItr(GetWorld());
		FVector BoxOrigin, BoxExtent;
		for (ActorsItr; ActorsItr; ++ActorsItr)
		{
			ActorsItr->GetActorBounds(false, BoxOrigin, BoxExtent);

			if (GetIsInSelection(Start, End, BoxOrigin, BoxExtent))
				Actors.Add(*ActorsItr);
		}
	}
	else
	{
		AActor* Actor = GetSingleSelection(Start);
		if (IsValid(Actor) && Actor->IsA(ABuilding::StaticClass()))
			Actors.Add(Cast<ABuilding>(Actor));
	}

	return Actors;
}