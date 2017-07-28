// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/Unit.h"
#include "Knight.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AKnight : public AUnit
{
	GENERATED_BODY()
	
	public:
		AKnight();

		virtual float GetSize() override;
		virtual unsigned int GetLifeBarWidth() override;
};
