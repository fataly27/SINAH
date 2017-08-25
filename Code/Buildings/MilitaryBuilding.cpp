// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MilitaryBuilding.h"


AMilitaryBuilding::AMilitaryBuilding() : Super()
{
}

void AMilitaryBuilding::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		PlayerZone = NewObject<UPlayerZoneType>(this, UPlayerZoneType::StaticClass());
		OpponentZone = NewObject<UOpponentZoneType>(this, UOpponentZoneType::StaticClass());

		PlayerZone->Init(this);
		OpponentZone->Init(this);
	}
}

//Zones
UPlayerZoneType* AMilitaryBuilding::GetPlayerZone()
{
	return PlayerZone;
}
UOpponentZoneType* AMilitaryBuilding::GetOpponentZone()
{
	return OpponentZone;
}

//Side
void AMilitaryBuilding::SetSide(Side NewSide)
{
	Super::SetSide(NewSide);

	if (PlayerZone && OpponentZone && Role == ROLE_Authority)
	{
		PlayerZone->GetLifeZone()->SetReachLevel(1);
		PlayerZone->GetLifeZone()->SetEffectLevel(0);
		PlayerZone->GetSpeedZone()->SetReachLevel(1);
		PlayerZone->GetSpeedZone()->SetEffectLevel(0);

		OpponentZone->GetLifeZone()->SetReachLevel(1);
		OpponentZone->GetLifeZone()->SetEffectLevel(0);
		OpponentZone->GetSpeedZone()->SetReachLevel(1);
		OpponentZone->GetSpeedZone()->SetEffectLevel(0);
	}
}