// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "../MultiplayerState.h"
#include "../SkillsTree.h"
#include "EconomicBuilding.h"

AEconomicBuilding::AEconomicBuilding() : Super(), TimeSinceCounterPlunder(0.f)
{
	bPlundered = false;
	DefaultOutputInASecond = 10;
	ActualOutputInASecond = DefaultOutputInASecond;

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
			else if (TimeSinceCounterPlunder >= 1.f)
			{
				ReceiveDamages(ActualHeal, 0, RelatedMilitaryBuilding->GetSide(), RelatedMilitaryBuilding->GetPlayer());
				TimeSinceCounterPlunder -= 1.f;
			}
		}
	}
}

int AEconomicBuilding::GetOutputInASecond()
{
	if (Player && Player->GetSkillsTree())
	{
		if (bPlundered)
			return Player->GetSkillsTree()->GetRessourcesModifier() * Player->GetSkillsTree()->GetPlunderingModifier() * ActualOutputInASecond / 2.f;
		else
			return ActualOutputInASecond * Player->GetSkillsTree()->GetRessourcesModifier();
	}
	else
	{
		if (bPlundered)
			return ActualOutputInASecond / 2.f;
		else
			return ActualOutputInASecond;
	}
}
bool AEconomicBuilding::GetIsPlundered()
{
	return bPlundered;
}
int AEconomicBuilding::GetOutputForLevel(unsigned int Level, bool WithModifier)
{
	if (WithModifier && Player && Player->GetSkillsTree())
		return Player->GetSkillsTree()->GetRessourcesModifier() * (DefaultOutputInASecond + DefaultOutputInASecond * (Level - 1) / 2);
	else
		return DefaultOutputInASecond + DefaultOutputInASecond * (Level - 1) / 2;
}


void AEconomicBuilding::SetLevel(unsigned int Level)
{
	if (!bPlundered)
	{
		Super::SetLevel(Level);

		if (Role == ROLE_Authority)
			ActualOutputInASecond = GetOutputForLevel(CurrentLevel, false);
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
void AEconomicBuilding::ReceiveDamages(int Physic, int Magic, ESide AttackingSide, AMultiplayerState* AttackingPlayer)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		if (FMath::RandRange(1, 10) == 1)
			Multicast_ShowParticle(Explosion);

		int Damages = Physic + Magic;
		TimeSinceLastAttack = 0.f;

		if (CurrentLife <= Damages)
		{
			SetSide(AttackingSide, AttackingPlayer);
			if (RelatedMilitaryBuilding && AttackingSide == RelatedMilitaryBuilding->GetSide())
			{
				if (!bPlundered)
				{
					if (Player && Player->GetSkillsTree())
					{
						int ChancesToKeepHigherLevel = Player->GetSkillsTree()->GetConversionModifier();
						int LevelReset = 1;

						while (ChancesToKeepHigherLevel > 0)
						{
							if (FMath::RandRange(1, 100) <= ChancesToKeepHigherLevel)
								LevelReset++;
							ChancesToKeepHigherLevel -= 100;
						}

						SetLevel(LevelReset);
					}
					else
						SetLevel(1);
				}
				bPlundered = false;
			}
			else if (RelatedMilitaryBuilding)
			{
				SetLevel(CurrentLevel - 1);
				bPlundered = true;
				TimeSinceCounterPlunder = 0.f;
			}
			else
			{
				if (Player && Player->GetSkillsTree())
				{
					int ChancesToKeepHigherLevel = Player->GetSkillsTree()->GetConversionModifier();
					int LevelReset = 1;

					while (ChancesToKeepHigherLevel > 0)
					{
						if (FMath::RandRange(1, 100) <= ChancesToKeepHigherLevel)
							LevelReset++;
						ChancesToKeepHigherLevel -= 100;
					}

					SetLevel(LevelReset);
				}
				else
					SetLevel(1);
			}
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
	DOREPLIFETIME(AEconomicBuilding, DefaultOutputInASecond);
	DOREPLIFETIME(AEconomicBuilding, ActualOutputInASecond);
}