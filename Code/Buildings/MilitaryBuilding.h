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
		void ChangeDecals(bool bNormal = true);

		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpPlayerLifeZoneEffect();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpOpponentLifeZoneEffect();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpPlayerSpeedZoneEffect();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpOpponentSpeedZoneEffect();

		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpPlayerLifeZoneReach();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpOpponentLifeZoneReach();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpPlayerSpeedZoneReach();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_LevelUpOpponentSpeedZoneReach();

		ULifeZone* GetPlayerLifeZone();
		ULifeZone* GetOpponentLifeZone();
		USpeedZone* GetPlayerSpeedZone();
		USpeedZone* GetOpponentSpeedZone();

		//Points
		int GetPoints();
		void AddPoints(unsigned int Add);
		void UseOnePoint();

		//Level
		virtual void LevelUp() override;

		//ESide
		virtual void SetSide(ESide NewSide) override;
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_SetMilitarySide();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_ChangeDecals();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:
		ULifeZone* PlayerLifeZone;
		ULifeZone* OpponentLifeZone;
		USpeedZone* PlayerSpeedZone;
		USpeedZone* OpponentSpeedZone;

		UPROPERTY(Replicated)
			int Points;
};
