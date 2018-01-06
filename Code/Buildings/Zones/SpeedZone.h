// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Zone.h"
#include "SpeedZone.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API USpeedZone : public UZone
{
	GENERATED_BODY()
	
	public:
		USpeedZone();
		void Init(bool bPlayer);

		float GetSpeedMultiplicator(AUnit* Unit, AMilitaryBuilding* Building);

	protected:
		float BaseSpeedMultiplicator;
};
