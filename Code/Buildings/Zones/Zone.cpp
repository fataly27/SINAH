// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Zone.h"

UZone::UZone()
{
	SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}
void UZone::Init(bool IsPlayer, float Reach)
{
	IsForPlayer = IsPlayer;

	if(IsForPlayer)
		MyDecalMaterial = UMaterialInstanceDynamic::Create(BaseDecalPlayerMaterial, this);
	else
		MyDecalMaterial = UMaterialInstanceDynamic::Create(BaseDecalOpponentMaterial, this);

	SetDecalMaterial(MyDecalMaterial);

	BaseReach = Reach;

	MaxReachLevel = 5;
	SetReachLevel(1);
	MaxEffectLevel = 4;
	SetEffectLevel(0);
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

	DecalSize = FVector(300, GetReach() * 100, GetReach() * 100);
}
void UZone::SetEffectLevel(unsigned int Level)
{
	if (Level > MaxEffectLevel)
		CurrentEffectLevel = MaxEffectLevel;
	else
		CurrentEffectLevel = Level;

	if(MyDecalMaterial)
		MyDecalMaterial->SetScalarParameterValue("Opacity", 0.4f * CurrentEffectLevel / MaxEffectLevel);
}

unsigned int UZone::GetCurrentEffectLevel()
{
	return CurrentEffectLevel;
}
unsigned int UZone::GetCurrentReachLevel()
{
	return CurrentReachLevel;
}
unsigned int UZone::GetMaxEffectLevel()
{
	return MaxEffectLevel;
}
unsigned int UZone::GetMaxReachLevel()
{
	return MaxReachLevel;
}


//Replication
void UZone::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UZone, IsForPlayer);

	DOREPLIFETIME(UZone, BaseReach);
	DOREPLIFETIME(UZone, MaxEffectLevel);
	DOREPLIFETIME(UZone, CurrentEffectLevel);
	DOREPLIFETIME(UZone, MaxReachLevel);
	DOREPLIFETIME(UZone, CurrentReachLevel);
}