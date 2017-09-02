// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/MilitaryBuilding.h"
#include "SmallMilitaryBuilding.generated.h"

/**
*
*/
UCLASS()
class SINAH_API ASmallMilitaryBuilding : public AMilitaryBuilding
{
	GENERATED_BODY()

	public:
		ASmallMilitaryBuilding();

		virtual float GetSize();
		virtual float GetHalfHeight();
		virtual unsigned int GetLifeBarWidth();
};
