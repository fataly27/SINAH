// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SpawnWidget.h"

void USpawnWidget::SetBuildingLevel(int Level)
{
	OneEnabled = Level >= 1;
	TwoEnabled = Level >= 2;
	ThreeEnabled = Level >= 3;
	FourEnabled = Level >= 4;
	FiveEnabled = Level >= 5;
}
void USpawnWidget::SetColor(FLinearColor NewColor)
{
	Color = NewColor;
}
void USpawnWidget::SetSpawnVisibility(bool bVisible)
{
	if (bVisible)
	{
		ListVisibility = ESlateVisibility::Visible;
		DetailsVisibility = ESlateVisibility::Visible;
	}
	else
	{
		ListVisibility = ESlateVisibility::Hidden;
		DetailsVisibility = ESlateVisibility::Hidden;
	}
}

void USpawnWidget::SetSpawnDetailsEnabled(int CurrentFood, int CurrentCells, int CurrentMetal, int CurrentCristals)
{
	int IntFood = FCString::Atoi(*Food);
	int IntMetal = FCString::Atoi(*Metal);
	int IntCells = FCString::Atoi(*Cells);
	int IntCristals = FCString::Atoi(*Cristals);

	FoodEnabled = IntFood <= CurrentFood;
	MetalEnabled = IntMetal <= CurrentMetal;
	CellsEnabled = IntCells <= CurrentCells;
	CristalsEnabled = IntCristals <= CurrentCristals;
}

void USpawnWidget::SetRessourcesRequired(int NewFood, int NewCells, int NewMetal, int NewCristals)
{
	Food = FString::FromInt(NewFood);
	Cells = FString::FromInt(NewCells);
	Metal = FString::FromInt(NewMetal);
	Cristals = FString::FromInt(NewCristals);
}

void USpawnWidget::SetPVs(int Max)
{
	PVs = FString::FromInt(Max);
}
void USpawnWidget::SetTheAttack(int Physic, int Magic)
{
	TheAttack = FString::FromInt(Physic).Append(" / ").Append(FString::FromInt(Magic));
}
void USpawnWidget::SetDefense(int Physic, int Magic)
{
	Defense = FString::FromInt(Physic).Append(" / ").Append(FString::FromInt(Magic));
}
void USpawnWidget::SetSpeed(int NewSpeed)
{
	Speed = FString::FromInt(NewSpeed).Append(" m/s");
}
void USpawnWidget::SetFieldOfSight(int NewFieldOfSight)
{
	FieldOfSight = FString::FromInt(NewFieldOfSight).Append(" m");
}
void USpawnWidget::SetRange(int NewRange)
{
	Range = FString::FromInt(NewRange).Append(" m");
}
void USpawnWidget::SetFoodEaten(int NewFoodEaten)
{
	FoodEaten = FString::FromInt(NewFoodEaten).Append(" /s");
}
void USpawnWidget::SetUnitName(FName Name)
{
	UnitSelectedName = Name;
}