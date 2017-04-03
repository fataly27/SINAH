// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "PlayerHUD.h"


APlayerHUD::APlayerHUD() : StartMousePos(0.f, 0.f), CurrentMousePos(0.f, 0.f), BoxDisplayed(TypeBox::None)
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

	IsPlayerBlue(true);
}

void APlayerHUD::DrawHUD()
{
	if (BoxDisplayed == TypeBox::Select)
	{
		if (PlayerIsBlue)
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(80, 80, 255));
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(80, 80, 255));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(80, 80, 255));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(80, 80, 255));

			DrawRect(FLinearColor(0, 0, 1, 0.15f), StartMousePos.X, StartMousePos.Y, CurrentMousePos.X - StartMousePos.X, CurrentMousePos.Y - StartMousePos.Y);
		}
		else
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 80, 80));
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 80, 80));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 80, 80));
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 80, 80));

			DrawRect(FLinearColor(1, 0, 0, 0.15f), StartMousePos.X, StartMousePos.Y, CurrentMousePos.X - StartMousePos.X, CurrentMousePos.Y - StartMousePos.Y);
		}

		TArray<AUnit*> Units;
		TArray<ABuilding*> Buildings;

		GetActorsInSelectionRectangle<AUnit>(StartMousePos, CurrentMousePos, Units, false, false);
		GetActorsInSelectionRectangle<ABuilding>(StartMousePos, CurrentMousePos, Buildings, false, false);

		ActorsBeingSelected.Empty();
		for (int i = 0; i < Units.Num(); i++)
		{
			ActorsBeingSelected.Add(TScriptInterface<IGameElementInterface>(Units[i]));
		}
		for (int i = 0; i < Buildings.Num(); i++)
			ActorsBeingSelected.Add(TScriptInterface<IGameElementInterface>(Buildings[i]));
	}
	else if (BoxDisplayed == TypeBox::Target)
	{
		if (PlayerIsBlue)
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(150, 150, 255), 2.f);
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(150, 150, 255), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(150, 150, 255), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(150, 150, 255), 2.f);
		}
		else
		{
			DrawLine(StartMousePos.X, StartMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 150, 150), 2.f);
			DrawLine(StartMousePos.X, StartMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 150, 150), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, CurrentMousePos.X, StartMousePos.Y, FColor(255, 150, 150), 2.f);
			DrawLine(CurrentMousePos.X, CurrentMousePos.Y, StartMousePos.X, CurrentMousePos.Y, FColor(255, 150, 150), 2.f);
		}

		TArray<AUnit*> Units;
		TArray<ABuilding*> Buildings;

		GetActorsInSelectionRectangle<AUnit>(StartMousePos, CurrentMousePos, Units, false, false);
		GetActorsInSelectionRectangle<ABuilding>(StartMousePos, CurrentMousePos, Buildings, false, false);

		ActorsBeingSelected.Empty();
		for (int i = 0; i < Units.Num(); i++)
			ActorsBeingSelected.Add(TScriptInterface<IGameElementInterface>(Units[i]));
		for (int i = 0; i < Buildings.Num(); i++)
			ActorsBeingSelected.Add(TScriptInterface<IGameElementInterface>(Buildings[i]));
	}

	for (int i(0); i < ActorsSelected.Num(); i++)
	{
		if (ActorsSelected[i]->IsSelected())
		{
			FVector2D TopPosition(Project(ActorsSelected[i]->GetLocation() + FVector(0.f, 0.f, ActorsSelected[i]->GetHalfHeight())));
			float Percent = float(ActorsSelected[i]->GetCurrentLife()) / float(ActorsSelected[i]->GetMaxLife());
			float HeightOfRects = FMath::RoundHalfFromZero(50.f / CurrentZoom) + 1.f;
			float WidthOfRect = 500.f / CurrentZoom;
			float Offset = 100.f / CurrentZoom + 1.f;

			if (ActorsSelected[i]->TellIfImBlue())
			{
				DrawRect(FLinearColor(0, 0, 1, 0.25f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect, HeightOfRects);
				DrawRect(FLinearColor(0, 0, 1, 0.75f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect * Percent, HeightOfRects);
			}
			else
			{
				DrawRect(FLinearColor(1, 0, 0, 0.25f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect, HeightOfRects);
				DrawRect(FLinearColor(1, 0, 0, 0.75f), TopPosition.X - WidthOfRect / 2, TopPosition.Y - Offset - HeightOfRects, WidthOfRect * Percent, HeightOfRects);
			}
		}
	}

	if (ActorsSelected.IsValidIndex(0) && ActorsSelected[0].GetObject()->IsA(AUnit::StaticClass()))
	{
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->TellIfImBlue() == PlayerIsBlue && CurrentUnit->GetMode() != Modes::Defense)
			{
				TArray<FVector> Destinations = CurrentUnit->GetDestinations();
				for (int j(0); j < Destinations.Num(); j++)
				{
					FVector2D PositionOnScreen(Project(Destinations[j]));
					if (PlayerIsBlue)
						DrawTextureSimple(BlueDestinationTexture, PositionOnScreen.X - BlueDestinationTexture->GetSurfaceWidth() / 2, PositionOnScreen.Y - BlueDestinationTexture->GetSurfaceHeight() / 2);
					else
						DrawTextureSimple(RedDestinationTexture, PositionOnScreen.X - RedDestinationTexture->GetSurfaceWidth() / 2, PositionOnScreen.Y - RedDestinationTexture->GetSurfaceHeight() / 2);
				}
			}
		}

		TArray<TScriptInterface<IGameElementInterface>> AllTargets;
		TArray<int> NumberOfOccurences;
		for (int i(0); i < ActorsSelected.Num(); i++)
		{
			AUnit* CurrentUnit = Cast<AUnit>(ActorsSelected[i].GetObject());
			if (CurrentUnit->TellIfImBlue() == PlayerIsBlue)
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
			float Offset = 100.f / CurrentZoom + 1.f;
			if (NumberOfOccurences[i] == ActorsSelected.Num())
			{
				if (PlayerIsBlue)
					DrawTextureSimple(BlueTargetTexture, TopPosition.X - BlueTargetTexture->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - BlueTargetTexture->GetSurfaceHeight() * Scale / 2, Scale);
				else
					DrawTextureSimple(RedTargetTexture, TopPosition.X - RedTargetTexture->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - RedTargetTexture->GetSurfaceHeight() * Scale / 2, Scale);
			}
			else
			{
				if (PlayerIsBlue)
					DrawTextureSimple(BlueTargetTextureLight, TopPosition.X - BlueTargetTextureLight->GetSurfaceWidth() * Scale / 2, TopPosition.Y - Offset - BlueTargetTextureLight->GetSurfaceHeight() * Scale / 2, Scale);
				else
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
void APlayerHUD::ShouldDisplayBox(TypeBox Display)
{
	BoxDisplayed = Display;
}
void APlayerHUD::IsPlayerBlue(bool color)
{
	PlayerIsBlue = color;
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