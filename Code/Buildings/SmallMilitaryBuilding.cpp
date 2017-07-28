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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/LittleBuilding/LittleMilitaryBuilding.LittleMilitaryBuilding'"));

	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingBlueMaterialRessource(TEXT("UMaterial'/Game/Meshes/LittleBuilding/Blue_Material.Blue_Material'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingRedMaterialRessource(TEXT("UMaterial'/Game/Meshes/LittleBuilding/Red_Material.Red_Material'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingNeutralMaterialRessource(TEXT("UMaterial'/Game/Meshes/LittleBuilding/Grey_Material.Grey_Material'"));

	StaticMesh = StaticMeshRessource.Object;

	BuildingBlueMaterial = BuildingBlueMaterialRessource.Object;
	BuildingRedMaterial = BuildingRedMaterialRessource.Object;
	BuildingNeutralMaterial = BuildingNeutralMaterialRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);

	SelectionMark->SetWorldScale3D(FVector(1.f, 3.f, 3.f));
}

float ASmallMilitaryBuilding::GetSize()
{
	return 410.f;
}
float ASmallMilitaryBuilding::GetHalfHeight()
{
	return 2000.f;
}
unsigned int ASmallMilitaryBuilding::GetLifeBarWidth()
{
	return 2500;
}