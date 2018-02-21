// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "EconomicBuilding.h"

AEconomicBuilding::AEconomicBuilding() : Super(), TimeSinceCounterPlunder(0.f)
{
	bPlundered = false;
	DefaultOutputInHalfASecond = 4;
	ActualOutputInHalfASecond = DefaultOutputInHalfASecond;

	DefaultMaxLife = 500;
	DefaultHeal = 4;
	DefaultFieldOfSight = 9.f;

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
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		TimeSinceCounterPlunder += DeltaTime;

		if (bPlundered && RelatedMilitaryBuilding)
		{
			if (RelatedMilitaryBuilding->GetSide() == MySide)
				bPlundered = false;
			else if (TimeSinceCounterPlunder >= 0.5f)
			{
				ReceiveDamages(ActualHeal, 0, RelatedMilitaryBuilding->GetSide());
				TimeSinceCounterPlunder -= 0.5f;
			}
		}
	}
}

int AEconomicBuilding::GetOutputInHalfASecond()
{
	if (bPlundered)
		return ActualOutputInHalfASecond / 2;
	else
		return ActualOutputInHalfASecond;
}
bool AEconomicBuilding::GetIsPlundered()
{
	return bPlundered;
}
int AEconomicBuilding::GetOutputForLevel(unsigned int Level)
{
	return DefaultOutputInHalfASecond + DefaultOutputInHalfASecond * (Level - 1) / 2;
}


void AEconomicBuilding::SetLevel(unsigned int Level)
{
	if (!bPlundered)
	{
		Super::SetLevel(Level);

		if (Role == ROLE_Authority)
			ActualOutputInHalfASecond = GetOutputForLevel(CurrentLevel);
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
void AEconomicBuilding::ReceiveDamages(int Physic, int Magic, ESide AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		int Damages = Physic + Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= Damages)
		{
			if (RelatedMilitaryBuilding && AttackingSide == RelatedMilitaryBuilding->GetSide())
			{
				if (!bPlundered)
					SetLevel(1);
				bPlundered = false;
			}
			else if (RelatedMilitaryBuilding)
			{
				SetLevel(CurrentLevel - 1);
				bPlundered = true;
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

	DOREPLIFETIME(AEconomicBuilding, bPlundered);
	DOREPLIFETIME(AEconomicBuilding, DefaultOutputInHalfASecond);
	DOREPLIFETIME(AEconomicBuilding, ActualOutputInHalfASecond);
}