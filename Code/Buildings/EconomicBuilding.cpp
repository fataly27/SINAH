// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "EconomicBuilding.h"

AEconomicBuilding::AEconomicBuilding() : Super(), TimeSinceCounterPlunder(0.f)
{
	IsPlundered = false;
	DefaultOutputInHalfASecond = 10;
	ActualOutputInHalfASecond = DefaultOutputInHalfASecond;

	DefaultMaxLife = 1000;
	DefaultHeal = 10;
	DefaultFieldOfSight = 5.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	LevelMax = 4;

	SetActorScale3D(FVector(0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/EconomicBuilding.EconomicBuilding'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);
}

void AEconomicBuilding::Tick(float DeltaTime)
{
	if (Role == ROLE_Authority)
	{
		TimeSinceCounterPlunder += DeltaTime;

		if (IsPlundered && RelatedMilitaryBuilding && TimeSinceCounterPlunder >= 0.5f)
		{
			ReceiveDamages(ActualHeal, 0, RelatedMilitaryBuilding->GetSide());
			TimeSinceCounterPlunder -= 0.5f;
		}
	}
}

unsigned int AEconomicBuilding::GetOutputInHalfASecond()
{
	if (IsPlundered)
		return ActualOutputInHalfASecond / 2;
	else
		return ActualOutputInHalfASecond;
}


void AEconomicBuilding::SetLevel(unsigned int Level)
{
	if (IsPlundered)
	{
		Super::SetLevel(Level);

		if (Role == ROLE_Authority)
			ActualOutputInHalfASecond = DefaultOutputInHalfASecond * FGenericPlatformMath::Sqrt(CurrentLevel);
	}
}

float AEconomicBuilding::GetSize()
{
	return 282.f;
}
float AEconomicBuilding::GetHalfHeight()
{
	return 400.f;
}
unsigned int AEconomicBuilding::GetLifeBarWidth()
{
	return 2500;
}

//Attack
void AEconomicBuilding::ReceiveDamages(unsigned int Physic, unsigned int Magic, Side AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		unsigned int Damages = Physic + Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= Damages)
		{
			if (RelatedMilitaryBuilding && AttackingSide == RelatedMilitaryBuilding->GetSide())
			{
				if (!IsPlundered)
					SetLevel(1);
				IsPlundered = false;
			}
			else if (RelatedMilitaryBuilding)
			{
				SetLevel(CurrentLevel - 1);
				IsPlundered = true;
				TimeSinceCounterPlunder = 0.f;
			}
			SetSide(AttackingSide);
		}
		else
			CurrentLife -= Damages;
	}
}

//Replication
void AEconomicBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEconomicBuilding, IsPlundered);
	DOREPLIFETIME(AEconomicBuilding, DefaultOutputInHalfASecond);
	DOREPLIFETIME(AEconomicBuilding, ActualOutputInHalfASecond);
}