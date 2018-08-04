// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameBeforeStartingWidget.h"
#include "UpWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API UUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		void SetFood(int Food);
		void SetMetal(int Metal);
		void SetCells(int Cells);
		void SetCristals(int Cristals);

		void SetFoodChange(int Food);
		void SetMetalChange(int Metal);
		void SetCellsChange(int Cells);
		void SetCristalsChange(int Cristals);

		void SetMusicTitle(FString Info);
		void SetMusicCivs(TArray<ECivs> CivsArray);

		void SetTime(int BaseSeconds);
		void SetPoints(int Pts);

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "Amounts")
			FString AmountOfFood;
		UPROPERTY(BlueprintReadWrite, Category = "Amounts")
			FString AmountOfMetal;
		UPROPERTY(BlueprintReadWrite, Category = "Amounts")
			FString AmountOfCells;
		UPROPERTY(BlueprintReadWrite, Category = "Amounts")
			FString AmountOfCristals;

		UPROPERTY(BlueprintReadWrite, Category = "Changes")
			FString FoodChange;
		UPROPERTY(BlueprintReadWrite, Category = "Changes")
			FString MetalChange;
		UPROPERTY(BlueprintReadWrite, Category = "Changes")
			FString CellsChange;
		UPROPERTY(BlueprintReadWrite, Category = "Changes")
			FString CristalsChange;

		UPROPERTY(BlueprintReadWrite, Category = "Music")
			FString Title;
		UPROPERTY(BlueprintReadWrite, Category = "Music")
			FString Civs;

		UPROPERTY(BlueprintReadWrite, Category = "Timer")
			FString TheTimer;

		UPROPERTY(BlueprintReadWrite, Category = "Timer")
			int Points;
};
