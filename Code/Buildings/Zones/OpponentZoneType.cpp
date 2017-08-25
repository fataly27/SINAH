// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "OpponentZoneType.h"

UOpponentZoneType::UOpponentZoneType() : Super() {}
void UOpponentZoneType::Init(ABuilding* Building)
{
	Super::Init(Building, 5.f, 20, 9.f, 1.f);
}

int UOpponentZoneType::GetLifeEffectOnUnit(AUnit* Unit)
{
	return -(LifeZone->GetLifeModifier(Unit, AssociatedBuilding));
}
float UOpponentZoneType::GetSpeedEffectOnUnit(AUnit* Unit)
{
	return 1.f / (SpeedZone->GetSpeedMultiplicator(Unit, AssociatedBuilding));
}