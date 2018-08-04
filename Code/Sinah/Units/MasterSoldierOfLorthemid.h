// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "MasterSoldierOfLorthemid.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMasterSoldierOfLorthemid : public AUnit
{
	GENERATED_BODY()
	
	public:
		AMasterSoldierOfLorthemid();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
