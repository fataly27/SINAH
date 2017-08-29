// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "MilitaryBuilding.generated.h"

class ULifeZone;
class USpeedZone;

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
		virtual void ChangeDecals(bool Reverse);

		ULifeZone* GetPlayerLifeZone();
		ULifeZone* GetOpponentLifeZone();
		USpeedZone* GetPlayerSpeedZone();
		USpeedZone* GetOpponentSpeedZone();

		//Side
		virtual void SetSide(Side NewSide) override;

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:
		UPROPERTY(Replicated)
			ULifeZone* PlayerLifeZone;
		UPROPERTY(Replicated)
			ULifeZone* OpponentLifeZone;
		UPROPERTY(Replicated)
			USpeedZone* PlayerSpeedZone;
		UPROPERTY(Replicated)
			USpeedZone* OpponentSpeedZone;
};
