// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Building.h"
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

		virtual unsigned int GetOutputInHalfASecond();
		virtual void SetLevel(unsigned int Level) override;

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	protected:

		UPROPERTY(Replicated)
			bool IsPlundered;
		UPROPERTY(Replicated)
			unsigned int DefaultOutputInHalfASecond;
		UPROPERTY(Replicated)
			unsigned int ActualOutputInHalfASecond;
};
