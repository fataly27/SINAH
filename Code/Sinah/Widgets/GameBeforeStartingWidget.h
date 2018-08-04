// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Components/Button.h"
#include "../Components/Image.h"
#include "GameBeforeStartingWidget.generated.h"

class ASkill;
class ASkillsTree;

UENUM(BlueprintType)
	enum class ECivs : uint8
	{
		None,
		Amuh,
		Lorthemid,
		Zhark,
		Hovanion
	};

/**
 * 
 */
UCLASS()
class SINAH_API UGameBeforeStartingWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		void LaunchGame();
		int HasGameBeenLaunched();
		void SetSkillTree(ASkillsTree* Tree);

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "BeforeStartingChecks")
			bool GameLaunched = false;
		UPROPERTY(BlueprintReadWrite, Category = "BeforeStartingChecks")
			bool IsReady;
		UPROPERTY(BlueprintReadWrite, Category = "BeforeStartingChecks")
			ECivs CivChosen;
		UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
			int Points = 0;
		UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
			TArray<UButton*> Buttons;
		UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
			TMap<int, UImage*> Images;
		UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
			TArray<ASkill*> Skills;
		UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
			ASkill* CurrentSkill;

		UFUNCTION(BlueprintCallable)
			void SpendOnePoint();
		UFUNCTION(BlueprintCallable)
			void AddOnePoint();
		UFUNCTION(BlueprintCallable)
			void SelectSkill(int Index);
		UFUNCTION(BlueprintCallable)
			bool IsSkillEnabled(int Index);
		UFUNCTION(BlueprintCallable)
			void ClearSkills();
		UFUNCTION(BlueprintCallable)
			void SendSkillsUpdate();
};