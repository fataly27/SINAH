// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Building.h"


// Sets default values
ABuilding::ABuilding() : IsVisibleForOpponent(true), ImBlue(true)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultMaxLife = 500;
	DefaultHeal = 5;
	DefaultFieldOfSight = 10;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	LevelMax = 3;
	CurrentLevel = 0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SelectionMark = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionMark"));
	SelectionMark->SetupAttachment(RootComponent);

	SelectionMark->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	SelectionMark->SetWorldScale3D(FVector(1.f, 5.f, 5.f));

	static ConstructorHelpers::FObjectFinder<UMaterial> RedCircleAsset(TEXT("/Game/Materials/RedCircle.RedCircle"));
	RedCircle = RedCircleAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueCircleAsset(TEXT("/Game/Materials/BlueCircle.BlueCircle"));
	BlueCircle = BlueCircleAsset.Object;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	Unselect();
	AmIBlue(ImBlue);
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastAttack += DeltaTime;
	TimeSinceLastHeal += DeltaTime;

	if (TimeSinceLastAttack >= 10.f && TimeSinceLastHeal >= 0.5f && Role == ROLE_Authority && CurrentLevel != 0)
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

//Color
void ABuilding::AmIBlue(bool color)
{
	ImBlue = color;

	if (ImBlue)
	{
		SelectionMark->SetMaterial(0, BlueCircle);
		BuildingMesh->SetStaticMesh(StaticBlueMesh);
	}
	else
	{
		SelectionMark->SetMaterial(0, RedCircle);
		BuildingMesh->SetStaticMesh(StaticRedMesh);
	}
}
bool ABuilding::TellIfImBlue()
{
	return ImBlue;
}

//Attack
void ABuilding::ReceiveDamages(float Physic, float Magic)
{
	if (Role == ROLE_Authority)
	{
		CurrentLife -= Physic;
		CurrentLife -= Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= 0.f)
		{
			if (CurrentLevel == 0)
			{
				// Need to see who attacked ! --> Code need to be changed
			}
			else
				AmIBlue(!TellIfImBlue());
				
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

		ActualMaxLife = DefaultMaxLife * FGenericPlatformMath::Sqrt(CurrentLevel);
		CurrentLife = ActualMaxLife;
		ActualHeal = DefaultHeal * FGenericPlatformMath::Sqrt(CurrentLevel);

		if (CurrentLevel == 0)
		{
			ActualFieldOfSight = 0.f;
			Unselect();
		}
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
int ABuilding::GetLifeBarWidth()
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
int ABuilding::GetLevel()
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