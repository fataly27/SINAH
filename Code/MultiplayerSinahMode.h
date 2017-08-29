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

		Side GetPlayerSide(int ID);

	private:

		TArray<int> PlayersIDs;
};
