// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameBeforeStartingWidget.h"
#include "UObject/NoExportTypes.h"
#include "MusicItem.generated.h"

/**
*
*/
UCLASS()
class SINAH_API UMusicItem : public UObject
{
	GENERATED_BODY()

	public:
		UMusicItem();
		void Init(int Victory, int Epicness, TArray<ECivs> Civ, USoundWave* Sound, FString Info);

		int GetPercents(int Victory, int Epicness, ECivs Player, ECivs Opponent);
		void TurnPass();
		USoundWave* Play();
		FString GetTitle();
		TArray<ECivs> GetCivs();

	protected:
		int VictoryPoints;
		int EpicnessPoints;
		int Turns;
		TArray<ECivs> Civs;
		FString Title;

		UPROPERTY()
			USoundWave* Music;
};