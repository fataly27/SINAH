// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpawnEntityWidget.generated.h"

class USpawnWidget;
class AUnit;

/**
 * 
 */
UCLASS()
class SINAH_API USpawnEntityWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		void SetColor(FLinearColor NewColor, FLinearColor NewButtonColor);
		void SetUnit(TSubclassOf<AUnit> Class, FName Name, UTexture* Image);
		void SetMainInterface(USpawnWidget* Interface);
		void SetRessourcesRequired(int NewFood, int NewCells, int NewMetal, int NewCristals);

		void SetLevelRequired(int Level);
		void SetPVs(int Max);
		void SetTheAttack(int Physic, int Magic);
		void SetDefense(int Physic, int Magic);
		void SetSpeed(int NewSpeed);
		void SetFieldOfSight(int NewFieldOfSight);
		void SetRange(int NewRange);
		void SetFoodEaten(int NewFoodEaten);

		void SetEntityIsEnabled(int BuildingLevel, int CurrentFood, int CurrentCells, int CurrentMetal, int CurrentCristals);

		UFUNCTION(BlueprintCallable)
			void TransferData();

		UFUNCTION(BlueprintImplementableEvent)
			void SetUnitImage(const UTexture* Image);
	
	protected:
		UPROPERTY(BlueprintReadWrite, Category = "SpawnColor")
			FLinearColor Color;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnColor")
			FLinearColor ButtonColor;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnClass")
			TSubclassOf<AUnit> UnitClass;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnClass")
			FName UnitName;
		UPROPERTY(BlueprintReadWrite, Category = "SpawnEnabled")
			bool EntityEnabled;

		USpawnWidget* MainInterface;
		UTexture* UnitImage;

		int LevelRequired;

		int Food;
		int Cells;
		int Metal;
		int Cristals;

		int PVs;
		int TheAttackPhysic;
		int TheAttackMagic;
		int DefensePhysic;
		int DefenseMagic;
		int FieldOfSight;
		int Speed;
		int Range;
		int FoodEaten;
};
