// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SpeedZone.h"

USpeedZone::USpeedZone() {}
void USpeedZone::Init(float Reach, float SpeedMultiplicator)
{
	Super::Init(Reach);
	BaseSpeedMultiplicator = SpeedMultiplicator;
}

float USpeedZone::GetSpeedMultiplicator(AUnit* Unit, ABuilding* Building)
{
	if (FVector::Dist(Building->GetActorLocation(), Unit->GetActorLocation()) > GetReach() * 100)
		return 1.f;
	else
		return BaseSpeedMultiplicator * FGenericPlatformMath::Sqrt(CurrentEffectLevel + 1);
}