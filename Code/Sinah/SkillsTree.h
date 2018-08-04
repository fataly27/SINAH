// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SkillsTree.generated.h"

UCLASS()
class SINAH_API ASkill : public AActor
{
	GENERATED_BODY()

	public:
		ASkill();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

		UPROPERTY(Replicated, BlueprintReadWrite)
			bool Skill;
		UPROPERTY(Replicated, BlueprintReadWrite)
			ASkill* Parent;
		UPROPERTY(Replicated, BlueprintReadWrite)
			FText Title;
		UPROPERTY(Replicated, BlueprintReadWrite)
			FText Description;
};

/**
 * 
 */
UCLASS()
class SINAH_API ASkillsTree : public AActor
{
	GENERATED_BODY()

	public:
		ASkillsTree();

		virtual void BeginPlay() override;

		float GetPhysicAttackModifier();
		float GetMagicAttackModifier();
		float GetPhysicDefenseModifier();
		float GetMagicDefenseModifier();

		float GetUnitSightModifier();
		float GetBuildingSightModifier();
		float GetModeChangeModifier();
		float GetBuildingHealModifier();
		float GetSpawnUnityLifeModifier();

		float GetRessourcesModifier();
		float GetPlunderingModifier();
		int GetConversionModifier();
		bool GetInvisibilityEnabled();
		float GetInvisibilityTimeModifier();
		float GetInvisibilityCoolDownModifier();

		void SetPoints(int Pts);
		int SetSkills(TArray<ASkill*> SkillsChange);
		int SetSkills(TArray<bool> SkillsChange);
		TArray<ASkill*> GetSkills();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
	
	private:
		UPROPERTY(Replicated)
			int Points = 2;
		UPROPERTY(Replicated)
			TArray<ASkill*> Skills;

		/*bool MyPhysicAttack1 = false;
		bool MyPhysicAttack2 = false;
		bool MyPhysicAttack3 = false;
		bool MyMagicAttack1 = false;
		bool MyMagicAttack2 = false;
		bool MyMagicAttack3 = false;
		bool MyPhysicDefense1 = false;
		bool MyPhysicDefense2 = false;
		bool MyPhysicDefense3 = false;
		bool MyMagicDefense1 = false;
		bool MyMagicDefense2 = false;
		bool MyMagicDefense3 = false;

		bool MyUnitSight1 = false;
		bool MyUnitSight2 = false;
		bool MyUnitSight3 = false;
		bool MyBuildingSight1 = false;
		bool MyBuildingSight2 = false;
		bool MyBuildingSight3 = false;
		bool MyModeChange1 = false;
		bool MyModeChange2= false;
		bool MyModeChange3 = false;
		bool MyBuildingHeal1 = false;
		bool MyBuildingHeal2 = false;
		bool MyBuildingHeal3 = false;
		bool MySpawnUnityLife1 = false;
		bool MySpawnUnityLife2 = false;
		bool MySpawnUnityLife3 = false;

		bool MyRessources1 = false;
		bool MyRessources2 = false;
		bool MyRessources3 = false;
		bool MyPlundering1 = false;
		bool MyPlundering2 = false;
		bool MyPlundering3 = false;
		bool MyConversion1 = false;
		bool MyConversion2 = false;
		bool MyConversion3 = false;
		bool MyInvisibility = false;
		bool MyInvisibilityTime = false;
		bool MyInvisibilityCoolDown = false;*/
};
