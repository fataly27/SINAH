// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZoneType.h"
#include "PlayerZoneType.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API UPlayerZoneType : public UZoneType
{
	GENERATED_BODY()
	
	public:
		UPlayerZoneType();
		void Init(ABuilding* Building);

		virtual int GetLifeEffectOnUnit(AUnit* Unit) override;
		virtual float GetSpeedEffectOnUnit(AUnit* Unit) override;
};
