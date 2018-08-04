// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "SpecialisedSoldierOfLorthemid.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ASpecialisedSoldierOfLorthemid : public AUnit
{
	GENERATED_BODY()
	
	public:
		ASpecialisedSoldierOfLorthemid();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
