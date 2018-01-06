// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "GreatMilitaryBuilding.h"

AGreatMilitaryBuilding::AGreatMilitaryBuilding() : Super()
{
	DefaultMaxLife = 3000;
	DefaultHeal = 40;
	DefaultFieldOfSight = 20.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	CostInFood = { 150, 300, 1000, 2000, 5000, 10000};
	CostInCells = { 400, 800, 2000, 4000, 10000, 20000 };
	CostInMetal = { 1000, 2000, 4000, 8000, 20000, 40000 };
	CostInCristals = { 50, 250, 1000, 2000, 5000, 15000 };

	SetActorRotation(FRotator(0.f, 45.f, 0.f));
	SetActorScale3D(FVector(1.f));

	LevelMax = 7;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/GreatMilitaryBuilding.GreatMilitaryBuilding'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);
}

float AGreatMilitaryBuilding::GetSize()
{
	return 750.f;
}
float AGreatMilitaryBuilding::GetHalfHeight()
{
	return 2000.f;
}
unsigned int AGreatMilitaryBuilding::GetLifeBarWidth()
{
	return 6000;
}