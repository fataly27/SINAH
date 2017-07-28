// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "EconomicBuilding.h"

AEconomicBuilding::AEconomicBuilding() : Super()
{
	IsPlundered = false;
	DefaultOutputInHalfASecond = 0;
	ActualOutputInHalfASecond = DefaultOutputInHalfASecond;

	DefaultMaxLife = 1000;
	DefaultHeal = 10;
	DefaultFieldOfSight = 5.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualHeal = DefaultHeal;
	ActualFieldOfSight = DefaultFieldOfSight;
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
	Super();

	if (Role == ROLE_Authority)
		ActualOutputInHalfASecond = DefaultOutputInHalfASecond * FGenericPlatformMath::Sqrt(CurrentLevel);
}

//Replication
void AEconomicBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AEconomicBuilding, IsPlundered);
	DOREPLIFETIME(AEconomicBuilding, DefaultOutputInHalfASecond);
	DOREPLIFETIME(AEconomicBuilding, ActualOutputInHalfASecond);
}