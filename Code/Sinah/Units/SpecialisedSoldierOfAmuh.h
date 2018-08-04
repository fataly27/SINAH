// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "SpecialisedSoldierOfAmuh.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ASpecialisedSoldierOfAmuh : public AUnit
{
	GENERATED_BODY()
	
	public:
		ASpecialisedSoldierOfAmuh();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
