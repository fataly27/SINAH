// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SpawnWidget.h"
#include "SpawnEntityWidget.h"

void USpawnEntityWidget::SetColor(FLinearColor NewColor, FLinearColor NewButtonColor)
{
	Color = NewColor;
	ButtonColor = NewButtonColor;
}
void USpawnEntityWidget::SetUnit(TSubclassOf<AUnit> Class, FName Name, UTexture* Image)
{
	UnitClass = Class;
	UnitName = Name;
	UnitImage = Image;
	SetUnitImage(Image);
}
void USpawnEntityWidget::SetMainInterface(USpawnWidget* Interface)
{
	MainInterface = Interface;
}
void USpawnEntityWidget::SetRessourcesRequired(int NewFood, int NewCells, int NewMetal, int NewCristals)
{
	Food = NewFood;
	Cells = NewCells;
	Metal = NewMetal;
	Cristals = NewCristals;
}

void USpawnEntityWidget::SetLevelRequired(int Level)
{
	LevelRequired = Level;
}
void USpawnEntityWidget::SetPVs(int Max)
{
	PVs = Max;
}
void USpawnEntityWidget::SetTheAttack(int Physic, int Magic)
{
	TheAttackPhysic = Physic;
	TheAttackMagic = Magic;
}
void USpawnEntityWidget::SetDefense(int Physic, int Magic)
{
	DefensePhysic = Physic;
	DefenseMagic = Magic;
}
void USpawnEntityWidget::SetSpeed(int NewSpeed)
{
	Speed = NewSpeed;
}
void USpawnEntityWidget::SetFieldOfSight(int NewFieldOfSight)
{
	FieldOfSight = NewFieldOfSight;
}
void USpawnEntityWidget::SetRange(int NewRange)
{
	Range = NewRange;
}
void USpawnEntityWidget::SetFoodEaten(int NewFoodEaten)
{
	FoodEaten = NewFoodEaten;
}

void USpawnEntityWidget::TransferData()
{
	MainInterface->SetRessourcesRequired(Food, Cells, Metal, Cristals);

	MainInterface->SetPVs(PVs);
	MainInterface->SetTheAttack(TheAttackPhysic, TheAttackMagic);
	MainInterface->SetDefense(DefensePhysic, DefenseMagic);
	MainInterface->SetFieldOfSight(FieldOfSight);
	MainInterface->SetSpeed(Speed);
	MainInterface->SetRange(Range);
	MainInterface->SetFoodEaten(FoodEaten);

	MainInterface->SetDetailsImage(UnitImage);
}

void USpawnEntityWidget::SetEntityIsEnabled(int BuildingLevel, int CurrentFood, int CurrentCells, int CurrentMetal, int CurrentCristals)
{
	EntityEnabled = BuildingLevel >= LevelRequired && CurrentFood >= Food && CurrentCells >= Cells && CurrentMetal >= Metal && CurrentCristals >= Cristals;
}