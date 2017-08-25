// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Zone.h"

UZone::UZone() {}
void UZone::Init(float Reach)
{
	BaseReach = Reach;

	CurrentReachLevel = 1;
	MaxReachLevel = 5;
	CurrentEffectLevel = 0;
	MaxEffectLevel = 4;
}

float UZone::GetReach()
{
	return BaseReach * FGenericPlatformMath::Sqrt(CurrentReachLevel);
}

void UZone::LevelUpReach()
{
	SetReachLevel(CurrentReachLevel + 1);
}
void UZone::LevelUpEffect()
{
	SetEffectLevel(CurrentEffectLevel + 1);
}

void UZone::SetReachLevel(unsigned int Level)
{
	if (Level > MaxReachLevel)
		CurrentReachLevel = MaxReachLevel;
	else
		CurrentReachLevel = Level;
}
void UZone::SetEffectLevel(unsigned int Level)
{
	if (Level > MaxEffectLevel)
		CurrentEffectLevel = MaxEffectLevel;
	else
		CurrentEffectLevel = Level;
}