// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "SpecialisedSoldierOfZhark.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ASpecialisedSoldierOfZhark : public AUnit
{
	GENERATED_BODY()
	
	public:
		ASpecialisedSoldierOfZhark();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
