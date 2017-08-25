// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LifeZone.h"
#include "SpeedZone.h"
#include "Zone.h"
#include "ZoneType.generated.h"

/**
 * 
 */
UCLASS(abstract)
class SINAH_API UZoneType : public UObject
{
	GENERATED_BODY()
	
	public:
		UZoneType();
		void Init(ABuilding* Building, float LifeReach, int LifeEffect, float SpeedReach, float SpeedEffect);

		virtual ULifeZone* GetLifeZone();
		virtual USpeedZone* GetSpeedZone();

		virtual int GetLifeEffectOnUnit(AUnit* Unit);
		virtual float GetSpeedEffectOnUnit(AUnit* Unit);

	protected:
		UPROPERTY()
			ULifeZone* LifeZone;
		UPROPERTY()
			USpeedZone* SpeedZone;

		UPROPERTY()
			ABuilding* AssociatedBuilding;
};
