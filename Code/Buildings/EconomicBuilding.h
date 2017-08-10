// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
#include "MilitaryBuilding.h"
#include "EconomicBuilding.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AEconomicBuilding : public ABuilding
{
	GENERATED_BODY()
	
	public:
		AEconomicBuilding();
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		virtual unsigned int GetOutputInHalfASecond();

		virtual void SetLevel(unsigned int Level) override;

		virtual float GetSize();
		virtual float GetHalfHeight();
		virtual unsigned int GetLifeBarWidth();

		//Attack
		virtual void ReceiveDamages(float Physic, float Magic, Side AttackingSide) override;

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:

		UPROPERTY(Replicated)
			bool IsPlundered;
		UPROPERTY(Replicated)
			unsigned int DefaultOutputInHalfASecond;
		UPROPERTY(Replicated)
			unsigned int ActualOutputInHalfASecond;
		UPROPERTY(EditAnywhere)
			AMilitaryBuilding* RelatedMilitaryBuilding;

		float TimeSinceCounterPlunder;
};
