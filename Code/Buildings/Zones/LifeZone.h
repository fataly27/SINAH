// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Zone.h"
#include "../../Units/Unit.h"
#include "../Building.h"
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
		void Init(float Reach, int LifeModifier);

		int GetLifeModifier(AUnit* Unit, ABuilding* Building);

	protected:
		int BaseLifeModifier;
};
