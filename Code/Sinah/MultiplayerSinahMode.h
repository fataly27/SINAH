// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameElementInterface.h"
#include "MultiplayerSinahMode.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMultiplayerSinahMode : public AGameMode
{
	GENERATED_BODY()

	public:
		AMultiplayerSinahMode();

		// Called every frame
		virtual void Tick(float DeltaSeconds) override;

		TArray<FUniqueNetIdRepl> GetPlayersIDs();
		ESide GetPlayerSide(FUniqueNetIdRepl ID);
		void SetIsReadyForPlayer(FUniqueNetIdRepl ID, bool Ready);

	private:
		TArray<FUniqueNetIdRepl> PlayersIDs;
		TMap<FUniqueNetIdRepl, bool> PlayersAreReady;
		bool DidGameStart;
};
