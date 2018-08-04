// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "MasterSoldierOfAmuh.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMasterSoldierOfAmuh : public AUnit
{
	GENERATED_BODY()
	
	public:
		AMasterSoldierOfAmuh();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
