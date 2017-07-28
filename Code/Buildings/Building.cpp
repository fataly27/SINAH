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

	SelectionMark = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionMark"));
	SelectionMark->SetupAttachment(RootComponent);

	SelectionMark->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UMaterial> RedCircleAsset(TEXT("/Game/Materials/RedCircle.RedCircle"));
	RedCircle = RedCircleAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueCircleAsset(TEXT("/Game/Materials/BlueCircle.BlueCircle"));
	BlueCircle = BlueCircleAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> NeutralCircleAsset(TEXT("/Game/Materials/NeutralCircle.NeutralCircle"));
	NeutralCircle = NeutralCircleAsset.Object;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	BuildingMesh->bReceivesDecals = false;
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
	SelectionMark->SetHiddenInGame(false);
}
void ABuilding::Unselect()
{
	Selected = false;
	SelectionMark->SetHiddenInGame(true);
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
		SelectionMark->SetMaterial(0, BlueCircle);
		BuildingMesh->SetMaterial(0, BuildingBlueMaterial);
	}
	else if (MySide == Side::Red)
	{
		SelectionMark->SetMaterial(0, RedCircle);
		BuildingMesh->SetMaterial(0, BuildingRedMaterial);
	}
	else
	{
		SelectionMark->SetMaterial(0, NeutralCircle);
		BuildingMesh->SetMaterial(0, BuildingNeutralMaterial);
	}
}
Side ABuilding::GetSide()
{
	return MySide;
}

//Attack
void ABuilding::ReceiveDamages(float Physic, float Magic, Side AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		CurrentLife -= Physic;
		CurrentLife -= Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= 0.f)
		{
			SetSide(AttackingSide);
			SetLevel(1);
		}
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