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

		float GetReach();

		void SetReachLevel(unsigned int Level);
		void SetEffectLevel(unsigned int Level);
		void LevelUpReach();
		void LevelUpEffect();

		unsigned int GetCurrentEffectLevel();
		unsigned int GetCurrentReachLevel();
		unsigned int GetMaxEffectLevel();
		unsigned int GetMaxReachLevel();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:
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
