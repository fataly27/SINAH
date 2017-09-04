// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "Zone.generated.h"

class AUnit;
class AMilitaryBuilding;

/**
 * 
 */
UCLASS(abstract)
class SINAH_API UZone : public UDecalComponent
{
	GENERATED_BODY()
	
	public:
		UZone();
		void Init(bool IsPlayer, float Reach);

		virtual float GetReach();

		virtual void SetReachLevel(unsigned int Level);
		virtual void SetEffectLevel(unsigned int Level);
		virtual void LevelUpReach();
		virtual void LevelUpEffect();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:
		UPROPERTY(Replicated)
			UMaterialInstanceDynamic* MyDecalMaterial;
		UMaterial* BaseDecalPlayerMaterial;
		UMaterial* BaseDecalOpponentMaterial;

		UPROPERTY(Replicated)
			bool IsForPlayer;

		UPROPERTY(Replicated)
			float BaseReach;

		UPROPERTY(Replicated)
			unsigned int MaxEffectLevel;
		UPROPERTY(Replicated)
			unsigned int CurrentEffectLevel;
		UPROPERTY(Replicated)
			unsigned int MaxReachLevel;
		UPROPERTY(Replicated)
			unsigned int CurrentReachLevel;
};
