// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpawnWidget.generated.h"

class USpawnEntityWidget;

/**
 * 
 */
UCLASS()
class SINAH_API USpawnWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		void SetBuildingLevel(int Level);
		void SetColor(FLinearColor NewColor);
		void SetSpawnVisibility(bool bVisible);

		void SetSpawnDetailsEnabled(int CurrentFood, int CurrentCells, int CurrentMetal, int CurrentCristals);

		void SetRessourcesRequired(int NewFood, int NewCells, int NewMetal, int NewCristals);

		void SetPVs(int Max);
		void SetTheAttack(int Physic, int Magic);
		void SetDefense(int Physic, int Magic);
		void SetSpeed(int NewSpeed);
		void SetFieldOfSight(int NewFieldOfSight);
		void SetRange(int NewRange);
		void SetFoodEaten(int NewFoodEaten);

		void SetUnitName(FName Name);

		UFUNCTION(BlueprintImplementableEvent)
			void AddEntity(const USpawnEntityWidget* Entity, const int LevelRequired, const int Line);
		UFUNCTION(BlueprintImplementableEvent)
			void SetDetailsImage(const UTexture* Image);
	
	protected:
		UPROPERTY(BlueprintReadWrite, Category = "SpawnColor")
			FLinearColor Color;

		UPROPERTY(BlueprintReadWrite, Category = "SpawnLevel")
			bool OneEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnLevel")
			bool TwoEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnLevel")
			bool ThreeEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnLevel")
			bool FourEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnLevel")
			bool FiveEnabled;

		UPROPERTY(BlueprintReadWrite, Category = "SpawnVisibility")
			ESlateVisibility ListVisibility;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnVisibility")
			ESlateVisibility DetailsVisibility;

		UPROPERTY(BlueprintReadWrite, Category = "SpawnEnabled")
			bool FoodEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnEnabled")
			bool CellsEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnEnabled")
			bool MetalEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnEnabled")
			bool CristalsEnabled;

		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Food;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Cells;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Metal;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Cristals;

		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString PVs;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString TheAttack;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Defense;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString FieldOfSight;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Speed;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString Range;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FString FoodEaten;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnContent")
			FName UnitSelectedName;
};
