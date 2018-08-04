// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "GreatMilitaryBuilding.h"

AGreatMilitaryBuilding::AGreatMilitaryBuilding() : Super()
{
	DefaultMaxLife = 3000;
	DefaultHeal = 45;
	DefaultFieldOfSight = 20.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	CostInFood = { 150, 300, 700, 1500 };
	CostInCells = { 400, 800, 2000, 3000 };
	CostInMetal = { 1000, 2000, 3000, 5000 };
	CostInCristals = { 50, 250, 1000, 2000 };

	SetActorScale3D(FVector(1.f));

	LevelMax = 5;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/GreatMilitaryBuilding2.GreatMilitaryBuilding2'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);
}

float AGreatMilitaryBuilding::GetSize()
{
	return 260.f;
}
float AGreatMilitaryBuilding::GetHalfHeight()
{
	return 1400.f;
}
unsigned int AGreatMilitaryBuilding::GetLifeBarWidth()
{
	return 6000;
}