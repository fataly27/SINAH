// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
		void Init(float Reach, float SpeedMultiplicator);

		float GetSpeedMultiplicator(AUnit* Unit, ABuilding* Building);

	protected:
		float BaseSpeedMultiplicator;
};
