// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LevelWidget.h"
#include "LevelEconomicWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ULevelEconomicWidget : public ULevelWidget
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintImplementableEvent, Category = "Invisible")
			void SetRelativeOutput(const float Current, const float Next);
		void SetOutput(int Current, int Next);
		void SetIsPlundered(bool Plundered);

		UFUNCTION(BlueprintImplementableEvent, Category = "BuildingTypes")
			void SetBuildingToFood();
		UFUNCTION(BlueprintImplementableEvent, Category = "BuildingTypes")
			void SetBuildingToMetal();
		UFUNCTION(BlueprintImplementableEvent, Category = "BuildingTypes")
			void SetBuildingToCells();
		UFUNCTION(BlueprintImplementableEvent, Category = "BuildingTypes")
			void SetBuildingToCristals();

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "LevelEconomic")
			bool IsPlundered;
		UPROPERTY(BlueprintReadWrite, Category = "LevelEconomic")
			FString CurrentOutput;
		UPROPERTY(BlueprintReadWrite, Category = "LevelEconomic")
			FString OutputAfterUpgrade;
};
