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
	DefaultFieldOfSight = 10.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;

	LevelMax = 4;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshRessource(TEXT("StaticMesh'/Game/Meshes/EconomicBuilding/EconomicBuilding.EconomicBuilding'"));
	StaticMesh = StaticMeshRessource.Object;

	BuildingMesh->SetStaticMesh(StaticMesh);

	SelectionMark->SetWorldScale3D(FVector(1.f, 3.f, 3.f));
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
	return 495.f;
}
float AEconomicBuilding::GetHalfHeight()
{
	return 700.f;
}
unsigned int AEconomicBuilding::GetLifeBarWidth()
{
	return 2500;
}

//Attack
void AEconomicBuilding::ReceiveDamages(float Physic, float Magic, Side AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		CurrentLife -= Physic;
		CurrentLife -= Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= 0.f)
		{
			if (RelatedMilitaryBuilding && AttackingSide == RelatedMilitaryBuilding->GetSide())
			{
				if(!IsPlundered)
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