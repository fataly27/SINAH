// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "SpecialisedSoldierOfHovanion.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ASpecialisedSoldierOfHovanion : public AUnit
{
	GENERATED_BODY()
	
	public:
		ASpecialisedSoldierOfHovanion();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
