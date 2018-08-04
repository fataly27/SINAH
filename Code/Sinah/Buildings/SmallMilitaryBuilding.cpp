// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SmallMilitaryBuilding.h"

ASmallMilitaryBuilding::ASmallMilitaryBuilding() : Super()
{
	DefaultMaxLife = 1000;
	DefaultHeal = 10;
	DefaultFieldOfSight = 15.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	CostInFood = { 150, 300 };
	CostInCells = { 400, 800 };
	CostInMetal = { 1000, 2000 };
	CostInCristals = { 50, 250 };

	SetActorScale3D(FVector(1.f));

	LevelMax = 3;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/LittleMilitaryBuilding.LittleMilitaryBuilding'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);
}

float ASmallMilitaryBuilding::GetSize()
{
	return 260.f;
}
float ASmallMilitaryBuilding::GetHalfHeight()
{
	return 1000.f;
}
unsigned int ASmallMilitaryBuilding::GetLifeBarWidth()
{
	return 3600;
}