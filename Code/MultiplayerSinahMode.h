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

		void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
		void HandleMatchHasEnded() override;
		void HandleLeavingMap() override;

		Side GetPlayerSide(int ID);

	private:
		TArray<int> PlayersIDs;
};
