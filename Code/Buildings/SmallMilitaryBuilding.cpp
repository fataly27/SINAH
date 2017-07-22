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

	LevelMax = 3;
	CurrentLevel = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBlueMeshRessource(TEXT("StaticMesh'/Game/Meshes/LittleBuilding/petronas_blue.petronas_blue'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticRedMeshRessource(TEXT("StaticMesh'/Game/Meshes/LittleBuilding/petronas_red.petronas_red'"));
	StaticBlueMesh = StaticBlueMeshRessource.Object;
	StaticRedMesh = StaticRedMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticBlueMesh);
	BuildingMesh->SetRelativeLocation(FVector(-40.f, 250.f, 0.f));
	BuildingMesh->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
	BuildingMesh->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
}

float ASmallMilitaryBuilding::GetSize()
{
	return 590.f;
}