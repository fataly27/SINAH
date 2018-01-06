// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Building.h"


// Sets default values
ABuilding::ABuilding() : bVisibleForOpponent(true), MySide(ESide::Neutral), bSelected(false)
{
	bReplicates = true;
	bAlwaysRelevant = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	CurrentLevel = 1;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingBlueMaterialRessource(TEXT("UMaterial'/Game/Materials/Colors/Blue_Material.Blue_Material'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingRedMaterialRessource(TEXT("UMaterial'/Game/Materials/Colors/Red_Material.Red_Material'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingNeutralMaterialRessource(TEXT("UMaterial'/Game/Materials/Colors/Grey_Material.Grey_Material'"));

	BuildingBlueMaterial = BuildingBlueMaterialRessource.Object;
	BuildingRedMaterial = BuildingRedMaterialRessource.Object;
	BuildingNeutralMaterial = BuildingNeutralMaterialRessource.Object;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	BuildingMesh->bReceivesDecals = false;
	BuildingMesh->SetRenderCustomDepth(false);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	Unselect();
	SetSide(MySide);
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastAttack += DeltaTime;
	TimeSinceLastHeal += DeltaTime;

	if (TimeSinceLastAttack >= 10.f && TimeSinceLastHeal >= 0.5f && Role == ROLE_Authority)
		Heal();
}

//Selection
void ABuilding::Select()
{
	bSelected = true;
	BuildingMesh->SetRenderCustomDepth(true);
}
void ABuilding::Unselect()
{
	bSelected = false;
	BuildingMesh->SetRenderCustomDepth(false);
}
bool ABuilding::IsSelected()
{
	return bSelected;
}

//ESide
void ABuilding::SetSide(ESide NewSide)
{
	Multicast_SetSide(NewSide);
}
void ABuilding::Multicast_SetSide_Implementation(ESide NewSide)
{
	Unselect();
	MySide = NewSide;

	if (MySide == ESide::Blue)
	{
		BuildingMesh->CustomDepthStencilValue = STENCIL_BLUE_OUTLINE;

		for (int i = 0; i < BuildingMesh->GetNumMaterials(); i++)
			BuildingMesh->SetMaterial(i, BuildingBlueMaterial);
	}
	else if (MySide == ESide::Red)
	{
		BuildingMesh->CustomDepthStencilValue = STENCIL_RED_OUTLINE;

		for (int i = 0; i < BuildingMesh->GetNumMaterials(); i++)
			BuildingMesh->SetMaterial(i, BuildingRedMaterial);
	}
	else
	{
		BuildingMesh->CustomDepthStencilValue = STENCIL_GREY_OUTLINE;

		for (int i = 0; i < BuildingMesh->GetNumMaterials(); i++)
			BuildingMesh->SetMaterial(i, BuildingNeutralMaterial);
	}

	CurrentLife = ActualMaxLife;
}
ESide ABuilding::GetSide()
{
	return MySide;
}

//Attack
void ABuilding::ReceiveDamages(int Physic, int Magic, ESide AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		int Damages = Physic + Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= Damages)
		{
			SetSide(AttackingSide);
			SetLevel(1);
		}
		else
			CurrentLife -= Damages;
	}
}

//Heal
void ABuilding::Heal()
{
	if (Role == ROLE_Authority)
	{
		CurrentLife += ActualHeal;

		if (CurrentLife > ActualMaxLife)
			CurrentLife = ActualMaxLife;

		TimeSinceLastHeal = 0.f;
	}
}

//LevelUp
void ABuilding::LevelUp()
{
	SetLevel(CurrentLevel + 1);
}
void ABuilding::SetLevel(unsigned int Level)
{
	if (Role == ROLE_Authority)
	{
		CurrentLevel = Level;
		if (CurrentLevel > LevelMax)
			CurrentLevel = LevelMax;
		if (CurrentLevel <= 0)
			CurrentLevel = 1;

		ActualMaxLife = DefaultMaxLife * FGenericPlatformMath::Sqrt(CurrentLevel);
		CurrentLife = ActualMaxLife;
		ActualHeal = DefaultHeal * FGenericPlatformMath::Sqrt(CurrentLevel);
		ActualFieldOfSight = DefaultFieldOfSight * FGenericPlatformMath::Sqrt(CurrentLevel);
	}
}

//Dying
bool ABuilding::IsPendingKill()
{
	return false;
}

//Statistics Getters
int ABuilding::GetMaxLife()
{
	return ActualMaxLife;
}
int ABuilding::GetCurrentLife()
{
	return CurrentLife;
}
float ABuilding::GetFieldOfSight()
{
	return ActualFieldOfSight;
}
float ABuilding::GetHalfHeight()
{
	return 1300.f;
}
unsigned int ABuilding::GetLifeBarWidth()
{
	return 2000;
}
float ABuilding::GetHeal()
{
	return ActualHeal;
}
float ABuilding::GetSize()
{
	return 0.f;
}
unsigned int ABuilding::GetLevel()
{
	return CurrentLevel;
}
unsigned int ABuilding::GetMaxLevel()
{
	return LevelMax;
}

int ABuilding::GetCostInFoodToLevel(int WantedLevel)
{
	if (WantedLevel > 1)
		return CostInFood[WantedLevel - 2];
	else
		return 0;
}
int ABuilding::GetCostInCellsToLevel(int WantedLevel)
{
	if (WantedLevel > 1)
		return CostInCells[WantedLevel - 2];
	else
		return 0;
}
int ABuilding::GetCostInMetalToLevel(int WantedLevel)
{
	if (WantedLevel > 1)
		return CostInMetal[WantedLevel - 2];
	else
		return 0;
}
int ABuilding::GetCostInCristalsToLevel(int WantedLevel)
{
	if (WantedLevel > 1)
		return CostInCristals[WantedLevel - 2];
	else
		return 0;
}

//Visibility
bool ABuilding::GetOpponentVisibility()
{
	return bVisibleForOpponent;
}
FVector ABuilding::GetLocation()
{
	return GetActorLocation();
}

//Replication
void ABuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuilding, CurrentLife);
	DOREPLIFETIME(ABuilding, ActualMaxLife);
	DOREPLIFETIME(ABuilding, ActualFieldOfSight);
	DOREPLIFETIME(ABuilding, ActualHeal);

	DOREPLIFETIME(ABuilding, CurrentLevel);

	DOREPLIFETIME(ABuilding, bVisibleForOpponent);
}