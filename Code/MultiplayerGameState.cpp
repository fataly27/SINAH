// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Buildings/MilitaryBuilding.h"
#include "MultiplayerGameState.h"
#include "MultiplayerSInahMode.h"
#include "GameElementInterface.h"


AMultiplayerGameState::AMultiplayerGameState() : Super(), GameActive(false), StateInfo("Waiting for your opponent"), CountDown(-1.f), CurrentTime(0.f), Winner(Side::Neutral)
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

	if(Role == ROLE_Authority && (IsMatchInProgress() || GetMatchState() == "Aborted"))
	{
		TActorIterator<AMilitaryBuilding> Building(GetWorld());
		Side FirstSide = Side::Neutral;
		bool IsGameEnded(true);
		bool FirstTime(true);
		for (Building; Building; ++Building)
		{
			if (!FirstTime)
			{
				if (Building->GetSide() != FirstSide && Building->GetSide() != Side::Neutral)
					IsGameEnded = false;
			}
			else if (Building->GetSide() != Side::Neutral)
			{
				FirstTime = false;
				FirstSide = Building->GetSide();
			}
		}

		if (GameActive)
			CurrentTime += DeltaTime;

		if (IsGameEnded && CountDown < -1.f)
		{
			GameActive = false;

			AMultiplayerSinahMode* Mode = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode());
			Mode->EndMatch();

			if (FirstSide == Side::Blue)
			{
				SetStatusInfo("Blue player has won !");
				Winner = Side::Blue;
			}
			else if (FirstSide == Side::Red)
			{
				SetStatusInfo("Red player has won !");
				Winner = Side::Red;
			}
		}
	}
}

bool AMultiplayerGameState::IsGameActive()
{
	return GameActive;
}

void AMultiplayerGameState::PreBeginGame()
{
	CountDown = 5.f;
}

void AMultiplayerGameState::BeginGame()
{
	GameActive = true;
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
	return CurrentTime;
}

//Replication
void AMultiplayerGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameState, GameActive);
	DOREPLIFETIME(AMultiplayerGameState, StateInfo);
	DOREPLIFETIME(AMultiplayerGameState, CurrentTime);
}