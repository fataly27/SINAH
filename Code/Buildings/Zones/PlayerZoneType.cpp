// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "PlayerZoneType.h"

UPlayerZoneType::UPlayerZoneType() : Super() {}
void UPlayerZoneType::Init(ABuilding* Building)
{
	Super::Init(Building, 10.f, 50, 18.f, 1.f);
}

int UPlayerZoneType::GetLifeEffectOnUnit(AUnit* Unit)
{
	return LifeZone->GetLifeModifier(Unit, AssociatedBuilding);
}
float UPlayerZoneType::GetSpeedEffectOnUnit(AUnit* Unit)
{
	return SpeedZone->GetSpeedMultiplicator(Unit, AssociatedBuilding);
}