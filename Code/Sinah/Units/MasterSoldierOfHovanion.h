// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "MasterSoldierOfHovanion.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMasterSoldierOfHovanion : public AUnit
{
	GENERATED_BODY()
	
	public:
		AMasterSoldierOfHovanion();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
