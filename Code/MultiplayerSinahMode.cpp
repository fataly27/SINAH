// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MousePlayerController.h"
#include "MainCamera.h"
#include "PlayerHUD.h"
#include "MultiplayerState.h"
#include "MultiplayerSinahMode.h"
#include "MultiplayerGameState.h"

AMultiplayerSinahMode::AMultiplayerSinahMode()
{
	PlayerControllerClass = AMousePlayerController::StaticClass();
	DefaultPawnClass = AMainCamera::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	PlayerStateClass = AMultiplayerState::StaticClass();
	GameStateClass = AMultiplayerGameState::StaticClass();
}

void AMultiplayerSinahMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NumPlayers == 2 && !HasMatchEnded() && GetMatchState() != "Aborted")
		GetGameState<AMultiplayerGameState>()->PreBeginGame();
}

void AMultiplayerSinahMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}
void AMultiplayerSinahMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
}

Side AMultiplayerSinahMode::GetPlayerSide(int ID)
{
	PlayersIDs.AddUnique(ID);

	int Index = PlayersIDs.Find(ID);

	if (Index == 0)
		return Side::Blue;
	else if (Index == 1)
		return Side::Red;
	else
		return Side::Neutral;
}