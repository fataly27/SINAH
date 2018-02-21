// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Buildings/MilitaryBuilding.h"
#include "GreatMilitaryBuilding.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AGreatMilitaryBuilding : public AMilitaryBuilding
{
	GENERATED_BODY()
	
	public:
		AGreatMilitaryBuilding();

		virtual float GetSize() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
};
