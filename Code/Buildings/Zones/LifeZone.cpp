// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "../MilitaryBuilding.h"
#include "../../Units/Unit.h"
#include "LifeZone.h"

ULifeZone::ULifeZone()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalPlayerMaterialAsset(TEXT("/Game/Materials/Zones/PlayerLifeZone.PlayerLifeZone"));
	BaseDecalPlayerMaterial = DecalPlayerMaterialAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalOpponentMaterialAsset(TEXT("/Game/Materials/Zones/OpponentLifeZone.OpponentLifeZone"));
	BaseDecalOpponentMaterial = DecalOpponentMaterialAsset.Object;
}
void ULifeZone::Init(bool bPlayer)
{
	if (bPlayer)
	{
		Super::Init(bPlayer, 10.f);
		BaseLifeModifier = 20;
	}
	else
	{
		Super::Init(bPlayer, 5.f);
		BaseLifeModifier = 8;
	}
}

int ULifeZone::GetLifeModifier(AUnit* Unit, AMilitaryBuilding* Building)
{
	if (FVector::Dist(Building->GetActorLocation(), Unit->GetActorLocation()) > GetReach() * 100)
		return 0;
	else
	{
		if(bForPlayer)
			return BaseLifeModifier * FGenericPlatformMath::Sqrt(CurrentEffectLevel);
		else
			return -BaseLifeModifier * FGenericPlatformMath::Sqrt(CurrentEffectLevel);
	}
}