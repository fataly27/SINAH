// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "../MilitaryBuilding.h"
#include "../../Units/Unit.h"
#include "SpeedZone.h"

USpeedZone::USpeedZone()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalPlayerMaterialAsset(TEXT("/Game/Materials/Zones/PlayerSpeedZone.PlayerSpeedZone"));
	BaseDecalPlayerMaterial = DecalPlayerMaterialAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalOpponentMaterialAsset(TEXT("/Game/Materials/Zones/OpponentSpeedZone.OpponentSpeedZone"));
	BaseDecalOpponentMaterial = DecalOpponentMaterialAsset.Object;
}
void USpeedZone::Init(bool Player)
{
	if(Player)
		Super::Init(Player, 18.f);
	else
		Super::Init(Player, 9.f);
	BaseSpeedMultiplicator = 1.f;
}

float USpeedZone::GetSpeedMultiplicator(AUnit* Unit, AMilitaryBuilding* Building)
{
	if (FVector::Dist(Building->GetActorLocation(), Unit->GetActorLocation()) > GetReach() * 100)
		return 1.f;
	else
	{
		if (IsForPlayer)
			return BaseSpeedMultiplicator * FGenericPlatformMath::Sqrt(CurrentEffectLevel + 1);
		else
			return 1.f / (BaseSpeedMultiplicator * FGenericPlatformMath::Sqrt(CurrentEffectLevel + 1));
	}
}