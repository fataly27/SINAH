// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelWidget.generated.h"

/**
 * 
 */
UCLASS(abstract)
class SINAH_API ULevelWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		void SetLevel(int CurrentLevel, int MaxLevel, FLinearColor NewColor);
		void SetRessourcesRequired(int Food, int Cells, int Metal, int Cristals, bool bFood, bool bCells, bool bMetal, bool bCristals);
		void SetIsLevelUpPossible(bool bPossible);

		void SetIsLevelVisible(bool bVisible);
		void SetAreDetailsVisible(bool bVisible);
		void SetIsLevelUpVisible(bool bVisible);

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			FString Level;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			FLinearColor Color;

		UPROPERTY(BlueprintReadWrite, Category = "Level")
			FString FoodRequired;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			bool IsThereEnoughFood;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			FString CellsRequired;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			bool IsThereEnoughCells;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			FString MetalRequired;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			bool IsThereEnoughMetal;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			FString CristalsRequired;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			bool IsThereEnoughCristals;

		UPROPERTY(BlueprintReadWrite, Category = "Level")
			bool IsLevelUpPossible;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			ESlateVisibility IsLevelVisible;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			ESlateVisibility AreDetailsVisible;
		UPROPERTY(BlueprintReadWrite, Category = "Level")
			ESlateVisibility IsLevelUpVisible;
};
