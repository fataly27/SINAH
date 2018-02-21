// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Zone.h"
#include "LifeZone.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ULifeZone : public UZone
{
	GENERATED_BODY()
	
	public:
		ULifeZone();
		void Init(bool bPlayer);

		int GetLifeModifier(AUnit* Unit, AMilitaryBuilding* Building);

	protected:
		int BaseLifeModifier;
};
