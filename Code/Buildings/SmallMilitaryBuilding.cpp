// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SmallMilitaryBuilding.h"

ASmallMilitaryBuilding::ASmallMilitaryBuilding() : Super()
{
	DefaultMaxLife = 1000;
	DefaultHeal = 10;
	DefaultFieldOfSight = 12.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	COST_IN_FOOD = { 150, 300, 1000 };
	COST_IN_CELLS = { 400, 800, 2000 };
	COST_IN_METAL = { 1000, 2000, 4000 };
	COST_IN_CRISTALS = { 50, 250, 1000 };

	SetActorScale3D(FVector(1.f));

	LevelMax = 4;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/LittleMilitaryBuilding.LittleMilitaryBuilding'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);
}

float ASmallMilitaryBuilding::GetSize()
{
	return 300.f;
}
float ASmallMilitaryBuilding::GetHalfHeight()
{
	return 1000.f;
}
unsigned int ASmallMilitaryBuilding::GetLifeBarWidth()
{
	return 3600;
}