// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Units/Unit.h"
#include "MasterSoldierOfZhark.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMasterSoldierOfZhark : public AUnit
{
	GENERATED_BODY()
	
	public:
		AMasterSoldierOfZhark();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
