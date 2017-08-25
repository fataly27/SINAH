// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Building.h"
#include "../../Units/Unit.h"
#include "Zone.generated.h"

/**
 * 
 */
UCLASS(abstract)
class SINAH_API UZone : public UObject
{
	GENERATED_BODY()
	
	public:
		UZone();
		void Init(float Reach);

		virtual float GetReach();

		virtual void SetReachLevel(unsigned int Level);
		virtual void SetEffectLevel(unsigned int Level);
		virtual void LevelUpReach();
		virtual void LevelUpEffect();

	protected:
		float BaseReach;

		unsigned int MaxEffectLevel;
		unsigned int CurrentEffectLevel;
		unsigned int MaxReachLevel;
		unsigned int CurrentReachLevel;
};
