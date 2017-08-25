// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "LifeZone.h"

ULifeZone::ULifeZone() {}
void ULifeZone::Init(float Reach, int LifeModifier)
{
	Super::Init(Reach);
	BaseLifeModifier = LifeModifier;
}

int ULifeZone::GetLifeModifier(AUnit* Unit, ABuilding* Building)
{
	if (FVector::Dist(Building->GetActorLocation(), Unit->GetActorLocation()) > GetReach() * 100)
		return 0;
	else
		return BaseLifeModifier * FGenericPlatformMath::Sqrt(CurrentEffectLevel);
}