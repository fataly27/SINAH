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
		void SetTitle(FString NewTitle, FLinearColor NewColor);
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
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString Title;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FLinearColor Color;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString PVs;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString Heal;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString TheAttack;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString Defense;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString Speed;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString FieldOfSight;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString Range;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			FString FoodEaten;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			ESlateVisibility UnitStats;
		UPROPERTY(BlueprintReadWrite, Category = "Stats")
			ESlateVisibility BuildingStats;
};
