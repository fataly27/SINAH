// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

/**
*
*/
UCLASS()
class SINAH_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		void SetColor(FLinearColor NewColor);
		void SetPVs(int Current, int Max);
		void SetHeal(int NewHeal);
		void SetTheAttack(int Physic, int Magic);
		void SetDefense(int Physic, int Magic);
		void SetSpeed(int NewSpeed);
		void SetFieldOfSight(int NewFieldOfSight);
		void SetRange(int NewRange);
		void SetFoodEaten(int NewFoodEaten);

		void SetStatsVisibility(ESlateVisibility Unit, ESlateVisibility Building);

		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "StatsColor")
			FLinearColor Color;

		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString PVs;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString Heal;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString TheAttack;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString Defense;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString Speed;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString FieldOfSight;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString Range;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContent")
			FString FoodEaten;

		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility PVsVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility HealVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility TheAttackVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility DefenseVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility SpeedVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility FieldOfSightVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility RangeVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "StatsContentVisibility")
			ESlateVisibility FoodEatenVisibility;

		UPROPERTY(BlueprintReadWrite, Category = "StatsGlobalVisibility")
			ESlateVisibility UnitStats;
		UPROPERTY(BlueprintReadWrite, Category = "StatsGlobalVisibility")
			ESlateVisibility BuildingStats;
};
