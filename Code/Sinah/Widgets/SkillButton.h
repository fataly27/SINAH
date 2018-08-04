// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SkillButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillDelegate, int, Index);

/**
 * 
 */
UCLASS()
class SINAH_API USkillButton : public UButton
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(BlueprintAssignable, Category = "Skill")
			FSkillDelegate OnSkillSelected;
		UPROPERTY(BlueprintReadWrite, Category = "Skill")
			int Index;
	
		UFUNCTION(BlueprintCallable)
			void SetIndex(int Idx);
		UFUNCTION()
			void ClickFunction();
};
