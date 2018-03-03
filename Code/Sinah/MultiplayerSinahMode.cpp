// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MousePlayerController.h"
#include "MainCamera.h"
#include "PlayerHUD.h"
#include "MultiplayerState.h"
#include "MultiplayerSinahMode.h"
#include "MultiplayerGameState.h"

AMultiplayerSinahMode::AMultiplayerSinahMode() : Super(), DidGameStart(false)
{
	PlayerControllerClass = AMousePlayerController::StaticClass();
	DefaultPawnClass = AMainCamera::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	PlayerStateClass = AMultiplayerState::StaticClass();
	GameStateClass = AMultiplayerGameState::StaticClass();
}

void AMultiplayerSinahMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (NumPlayers == 2 && PlayersIDs.Num() == 2 && !DidGameStart)
	{
		bool IsEveryPlayerReady(true);
		for (int i(0); i < PlayersIDs.Num(); i++)
		{
			if (!PlayersAreReady.Contains(PlayersIDs[i]) || !PlayersAreReady[PlayersIDs[i]])
				IsEveryPlayerReady = false;
		}

		if (IsEveryPlayerReady)
		{
			GetGameState<AMultiplayerGameState>()->PreBeginGame();
			DidGameStart = true;
		}
	}
}

ESide AMultiplayerSinahMode::GetPlayerSide(FUniqueNetIdRepl ID)
{
	PlayersIDs.AddUnique(ID);

	int Index = PlayersIDs.Find(ID);

	if (Index == 0)
		return ESide::Blue;
	else if (Index == 1)
		return ESide::Red;
	else
		return ESide::Neutral;
}

TArray<FUniqueNetIdRepl> AMultiplayerSinahMode::GetPlayersIDs()
{
	return PlayersIDs;
}

void AMultiplayerSinahMode::SetIsReadyForPlayer(FUniqueNetIdRepl ID, bool Ready)
{
	PlayersAreReady.Add(ID, Ready);
}