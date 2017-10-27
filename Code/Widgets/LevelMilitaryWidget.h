// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LevelWidget.h"
#include "LevelMilitaryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API ULevelMilitaryWidget : public ULevelWidget
{
	GENERATED_BODY()
	
	public:
		void SetPoints(int Pts);

		void SetPlayerLifeEffect(int Current, int Max);
		void SetPlayerSpeedEffect(int Current, int Max);
		void SetOpponentLifeEffect(int Current, int Max);
		void SetOpponentSpeedEffect(int Current, int Max);

		void SetPlayerLifeReach(int Current, int Max);
		void SetPlayerSpeedReach(int Current, int Max);
		void SetOpponentLifeReach(int Current, int Max);
		void SetOpponentSpeedReach(int Current, int Max);

	protected:
		int Points;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString String_Points;

		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentPlayerLifeEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentPlayerSpeedEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentOpponentLifeEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentOpponentSpeedEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentPlayerLifeReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentPlayerSpeedReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentOpponentLifeReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString CurrentOpponentSpeedReach;

		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxPlayerLifeEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxPlayerSpeedEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxOpponentLifeEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxOpponentSpeedEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxPlayerLifeReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxPlayerSpeedReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxOpponentLifeReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			FString MaxOpponentSpeedReach;

		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool PlayerLifePossibleEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool PlayerSpeedPossibleEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool OpponentLifePossibleEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool OpponentSpeedPossibleEffect;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool PlayerLifePossibleReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool PlayerSpeedPossibleReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool OpponentLifePossibleReach;
		UPROPERTY(BlueprintReadWrite, Category = "LevelMilitary")
			bool OpponentSpeedPossibleReach;
};
