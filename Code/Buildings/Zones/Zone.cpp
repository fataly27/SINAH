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
	SetReachLevel(5);
	MaxEffectLevel = 4;
	SetEffectLevel(4);
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
		MyDecalMaterial->SetScalarParameterValue("Opacity", 1.f / FMath::Sqrt(MaxEffectLevel) * FMath::Sqrt(CurrentEffectLevel));
}


//Replication
void UZone::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(UZone, MyDecalMaterial);

	DOREPLIFETIME(UZone, IsForPlayer);

	DOREPLIFETIME(UZone, BaseReach);
	DOREPLIFETIME(UZone, MaxEffectLevel);
	DOREPLIFETIME(UZone, CurrentEffectLevel);
	DOREPLIFETIME(UZone, MaxReachLevel);
	DOREPLIFETIME(UZone, CurrentReachLevel);
}