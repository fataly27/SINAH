// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "MilitaryBuilding.h"
#include "EconomicBuilding.generated.h"

class AMilitaryBuilding;

/**
 * 
 */
UCLASS(abstract)
class SINAH_API AEconomicBuilding : public ABuilding
{
	GENERATED_BODY()
	
	public:
		AEconomicBuilding();
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		int GetOutputInHalfASecond();
		bool GetIsPlundered();
		int GetOutputForLevel(unsigned int Level);

		virtual void SetLevel(unsigned int Level) override;

		virtual float GetSize() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;

		//Attack
		virtual void ReceiveDamages(int Physic, int Magic, ESide AttackingSide) override;

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:

		UPROPERTY(Replicated)
			bool bPlundered;
		UPROPERTY(Replicated)
			int DefaultOutputInHalfASecond;
		UPROPERTY(Replicated)
			int ActualOutputInHalfASecond;
		UPROPERTY(EditAnywhere)
			AMilitaryBuilding* RelatedMilitaryBuilding;

		float TimeSinceCounterPlunder;
};
