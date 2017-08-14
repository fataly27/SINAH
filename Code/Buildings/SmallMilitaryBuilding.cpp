// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SmallMilitaryBuilding.h"

ASmallMilitaryBuilding::ASmallMilitaryBuilding() : Super()
{
	DefaultMaxLife = 1000;
	DefaultHeal = 10;
	DefaultFieldOfSight = 20.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	COST_IN_FOOD = { 150, 500 };
	COST_IN_CELLS = { 400, 1200 };
	COST_IN_METAL = { 1000, 3000 };
	COST_IN_CRISTALS = { 50, 250 };

	LevelMax = 3;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/LittleBuilding/LittleMilitaryBuilding.LittleMilitaryBuilding'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);

	SelectionMark->SetWorldScale3D(FVector(1.f, 3.f, 3.f));
}

float ASmallMilitaryBuilding::GetSize()
{
	return 523.f;
}
float ASmallMilitaryBuilding::GetHalfHeight()
{
	return 2000.f;
}
unsigned int ASmallMilitaryBuilding::GetLifeBarWidth()
{
	return 3600;
}