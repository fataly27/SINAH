// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Building.h"


// Sets default values
ABuilding::ABuilding() : IsVisibleForOpponent(true), MySide(Side::Neutral), Selected(false)
{
	bReplicates = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	CurrentLevel = 1;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingBlueMaterialRessource(TEXT("UMaterial'/Game/Meshes/LittleBuilding/Blue_Material.Blue_Material'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingRedMaterialRessource(TEXT("UMaterial'/Game/Meshes/LittleBuilding/Red_Material.Red_Material'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingNeutralMaterialRessource(TEXT("UMaterial'/Game/Meshes/LittleBuilding/Grey_Material.Grey_Material'"));

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
	Selected = true;
	BuildingMesh->SetRenderCustomDepth(true);
}
void ABuilding::Unselect()
{
	Selected = false;
	BuildingMesh->SetRenderCustomDepth(false);
}
bool ABuilding::IsSelected()
{
	return Selected;
}

//Side
void ABuilding::SetSide(Side NewSide)
{
	Multicast_SetSide(NewSide);
}
void ABuilding::Multicast_SetSide_Implementation(Side NewSide)
{
	Unselect();
	MySide = NewSide;

	if (MySide == Side::Blue)
	{
		BuildingMesh->CustomDepthStencilValue = STENCIL_BLUE_OUTLINE;
		BuildingMesh->SetMaterial(0, BuildingBlueMaterial);
	}
	else if (MySide == Side::Red)
	{
		BuildingMesh->CustomDepthStencilValue = STENCIL_RED_OUTLINE;
		BuildingMesh->SetMaterial(0, BuildingRedMaterial);
	}
	else
	{
		BuildingMesh->CustomDepthStencilValue = STENCIL_GREY_OUTLINE;
		BuildingMesh->SetMaterial(0, BuildingNeutralMaterial);
	}

	CurrentLife = ActualMaxLife;
}
Side ABuilding::GetSide()
{
	return MySide;
}

//Attack
void ABuilding::ReceiveDamages(unsigned int Physic, unsigned int Magic, Side AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		unsigned int Damages = Physic + Magic;
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
void ABuilding::Server_LevelUp_Implementation()
{
	SetLevel(CurrentLevel + 1);
}
bool ABuilding::Server_LevelUp_Validate()
{
	return true;
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
unsigned int ABuilding::GetMaxLife()
{
	return ActualMaxLife;
}
unsigned int ABuilding::GetCurrentLife()
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

unsigned int ABuilding::GetCostInFoodToLevel(unsigned int WantedLevel)
{
	if (WantedLevel > 1)
		return COST_IN_FOOD[WantedLevel - 2];
	else
		return 0;
}
unsigned int ABuilding::GetCostInCellsToLevel(unsigned int WantedLevel)
{
	if (WantedLevel > 1)
		return COST_IN_CELLS[WantedLevel - 2];
	else
		return 0;
}
unsigned int ABuilding::GetCostInMetalToLevel(unsigned int WantedLevel)
{
	if (WantedLevel > 1)
		return COST_IN_METAL[WantedLevel - 2];
	else
		return 0;
}
unsigned int ABuilding::GetCostInCristalsToLevel(unsigned int WantedLevel)
{
	if (WantedLevel > 1)
		return COST_IN_CRISTALS[WantedLevel - 2];
	else
		return 0;
}

//Visibility
bool ABuilding::GetOpponentVisibility()
{
	return IsVisibleForOpponent;
}
FVector ABuilding::GetLocation()
{
	return GetActorLocation();
}

//Replication
void ABuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(ABuilding, CurrentLife);
	DOREPLIFETIME(ABuilding, ActualMaxLife);
	DOREPLIFETIME(ABuilding, ActualFieldOfSight);
	DOREPLIFETIME(ABuilding, ActualHeal);

	DOREPLIFETIME(ABuilding, IsVisibleForOpponent);
}