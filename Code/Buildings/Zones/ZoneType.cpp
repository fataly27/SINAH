// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "ZoneType.h"

UZoneType::UZoneType()
{
}
void UZoneType::Init(ABuilding* Building, float LifeReach, int LifeEffect, float SpeedReach, float SpeedEffect)
{
	LifeZone = NewObject<ULifeZone>(this, ULifeZone::StaticClass());
	SpeedZone = NewObject<USpeedZone>(this, USpeedZone::StaticClass());

	AssociatedBuilding = Building;

	LifeZone->Init(LifeReach, LifeEffect);
	SpeedZone->Init(SpeedReach, SpeedEffect);
}

ULifeZone* UZoneType::GetLifeZone()
{
	return LifeZone;
}
USpeedZone* UZoneType::GetSpeedZone()
{
	return SpeedZone;
}

int UZoneType::GetLifeEffectOnUnit(AUnit* Unit)
{
	return 0;
}
float UZoneType::GetSpeedEffectOnUnit(AUnit* Unit)
{
	return 0.f;
}