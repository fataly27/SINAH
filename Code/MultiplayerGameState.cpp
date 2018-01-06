// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Buildings/MilitaryBuilding.h"
#include "MultiplayerGameState.h"
#include "MultiplayerSInahMode.h"
#include "GameElementInterface.h"


AMultiplayerGameState::AMultiplayerGameState() : Super(), bGameActive(false), StateInfo("Waiting for your opponent"), CountDown(-1.f), CurrentTime(0.f), Winner(ESide::Neutral)
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
		ESide FirstSide = ESide::Neutral;
		bool bGameEnded(true);
		bool bFirstTime(true);
		for (Building; Building; ++Building)
		{
			if (!bFirstTime)
			{
				if (Building->GetSide() != FirstSide && Building->GetSide() != ESide::Neutral)
					bGameEnded = false;
			}
			else if (Building->GetSide() != ESide::Neutral)
			{
				bFirstTime = false;
				FirstSide = Building->GetSide();
			}
		}

		if (bGameActive)
			CurrentTime += DeltaTime;

		if (bGameEnded && CountDown < -1.f)
		{
			bGameActive = false;

			AMultiplayerSinahMode* Mode = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode());
			Mode->EndMatch();

			if (FirstSide == ESide::Blue)
			{
				SetStatusInfo("Blue player has won !");
				Winner = ESide::Blue;
			}
			else if (FirstSide == ESide::Red)
			{
				SetStatusInfo("Red player has won !");
				Winner = ESide::Red;
			}
		}
	}
}

bool AMultiplayerGameState::IsGameActive()
{
	return bGameActive;
}

void AMultiplayerGameState::PreBeginGame()
{
	CountDown = 5.f;
}

void AMultiplayerGameState::BeginGame()
{
	bGameActive = true;
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

	DOREPLIFETIME(AMultiplayerGameState, bGameActive);
	DOREPLIFETIME(AMultiplayerGameState, StateInfo);
	DOREPLIFETIME(AMultiplayerGameState, CurrentTime);
}