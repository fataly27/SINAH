// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MultiplayerSinahMode.h"

AMultiplayerSinahMode::AMultiplayerSinahMode()
{
	PlayerControllerClass = AMousePlayerController::StaticClass();
	DefaultPawnClass = AMainCamera::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	PlayerStateClass = AMultiplayerState::StaticClass();
}