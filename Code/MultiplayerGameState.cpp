// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MultiplayerGameState.h"


AMultiplayerGameState::AMultiplayerGameState() : Super(), GameBegan(false), StateInfo("Waiting for your opponent"), BeginTime(0), CountDown(-1.f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

void AMultiplayerGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CountDown > -1.f)
	{
		CountDown -= DeltaTime;
		if (CountDown <= -1.f)
			SetStatusInfo("");
		else if (CountDown <= 0.f)
		{
			SetStatusInfo("GO !");
			BeginGame();
		}
		else
			SetStatusInfo(FString::FromInt((int)(FMath::RoundFromZero(CountDown))));
	}
}

bool AMultiplayerGameState::DidGameBegin()
{
	return GameBegan;
}

void AMultiplayerGameState::PreBeginGame()
{
	CountDown = 5.f;
}

void AMultiplayerGameState::BeginGame()
{
	GameBegan = true;
	BeginTime = ElapsedTime;
}

FString AMultiplayerGameState::GetStatusInfo()
{
	return StateInfo;
}

void AMultiplayerGameState::SetStatusInfo(FString Text)
{
	StateInfo = Text;
}

int AMultiplayerGameState::GetTime()
{
	if (GameBegan)
		return ElapsedTime - BeginTime;
	else
		return 0.f;
}

//Replication
void AMultiplayerGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameState, GameBegan);
	DOREPLIFETIME(AMultiplayerGameState, StateInfo);
	DOREPLIFETIME(AMultiplayerGameState, BeginTime);
}