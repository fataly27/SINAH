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

		virtual float GetSize() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
};
