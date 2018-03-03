// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameBeforeStartingWidget.generated.h"

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

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "BeforeStartingChecks")
			bool IsReady;
		UPROPERTY(BlueprintReadWrite, Category = "BeforeStartingChecks")
			ECivs CivChosen;
};
