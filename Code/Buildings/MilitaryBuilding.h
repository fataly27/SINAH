// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "Zones/PlayerZoneType.h"
#include "Zones/OpponentZoneType.h"
#include "MilitaryBuilding.generated.h"

/**
 * 
 */
UCLASS(abstract)
class SINAH_API AMilitaryBuilding : public ABuilding
{
	GENERATED_BODY()
	
	public:
		AMilitaryBuilding();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		//Zones
		virtual UPlayerZoneType* GetPlayerZone();
		virtual UOpponentZoneType* GetOpponentZone();

		//Side
		virtual void SetSide(Side NewSide) override;

	protected:
		UPROPERTY()
			UPlayerZoneType* PlayerZone;
		UPROPERTY()
			UOpponentZoneType* OpponentZone;
};
