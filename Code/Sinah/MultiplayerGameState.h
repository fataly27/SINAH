// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Widgets/GameBeforeStartingWidget.h"
#include "MultiplayerGameState.generated.h"


USTRUCT()
struct FCivForPlayerStruct
{
	GENERATED_BODY()

	public:
		UPROPERTY()
			FUniqueNetIdRepl ID;
		UPROPERTY()
			ECivs Civ;
};
USTRUCT()
struct FPointsForPlayerStruct
{
	GENERATED_BODY()

	public:
		UPROPERTY()
			FUniqueNetIdRepl ID;
		UPROPERTY()
			int VictoryPoints;
		UPROPERTY()
			int EpicnessPoints;
};


/**
 * 
 */
UCLASS()
class SINAH_API AMultiplayerGameState : public AGameState
{
	GENERATED_BODY()
	
	public:
		AMultiplayerGameState();

		void Tick(float DeltaTime) override;

		bool IsGameActive();
		bool IsGameSoonActive();
		void PreBeginGame();
		void BeginGame();

		FString GetStatusInfo();
		void SetStatusInfo(FString Text);

		int GetTime();

		TArray<FPointsForPlayerStruct> GetPlayersPoints();

		FUniqueNetIdRepl GetWinner();
		FUniqueNetIdRepl GetIdBySide(ESide Side);
		void SetCivForPlayer(FUniqueNetIdRepl ID, ECivs Civ);
		TArray<FCivForPlayerStruct> GetAllCivs();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	private:
		UPROPERTY(Replicated)
			bool bGameActive;
		UPROPERTY(Replicated)
			bool bGameSoonActive;
		UPROPERTY(Replicated)
			FUniqueNetIdRepl Winner;
		UPROPERTY(Replicated)
			FString StateInfo;
		UPROPERTY(Replicated)
			float CurrentTime;
		float CountDown;

		UPROPERTY(Replicated)
			TArray<FPointsForPlayerStruct> PlayersPoints;
		
		UPROPERTY(Replicated)
			TArray<FCivForPlayerStruct> PlayersCiv;
};
