// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Buildings/MilitaryBuilding.h"
#include "Buildings/SmallMilitaryBuilding.h"
#include "Buildings/GreatMilitaryBuilding.h"
#include "Buildings/EconomicBuilding.h"
#include "Units/Unit.h"
#include "SkillsTree.h"
#include "MultiplayerState.h"
#include "MultiplayerGameState.h"
#include "MultiplayerSinahMode.h"
#include "GameElementInterface.h"


AMultiplayerGameState::AMultiplayerGameState() : Super(), bGameActive(false), bGameSoonActive(false), StateInfo("Waiting for your opponent"), CountDown(-1.f), CurrentTime(0.f)
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
		PlayersPoints.Empty();
		TArray<FUniqueNetIdRepl> PlayersIDs = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode())->GetPlayersIDs();
		for (int i(0); i < PlayersIDs.Num(); i++)
		{
			FPointsForPlayerStruct Struct;
			Struct.ID = PlayersIDs[i];
			Struct.VictoryPoints = 0;
			Struct.EpicnessPoints = 0;
			PlayersPoints.Add(Struct);
		}

		TActorIterator<AGreatMilitaryBuilding> GreatMilitaryBuilding(GetWorld());
		for (GreatMilitaryBuilding; GreatMilitaryBuilding; ++GreatMilitaryBuilding)
		{
			int VictoryPoints = 150;
			int EpicnessPoints = 15;
			FUniqueNetIdRepl ID = GetIdBySide(GreatMilitaryBuilding->GetSide());

			for (int i(0); i < PlayersIDs.Num(); i++)
			{
				if (PlayersPoints[i].ID == ID)
				{
					PlayersPoints[i].VictoryPoints += VictoryPoints;
					PlayersPoints[i].EpicnessPoints += EpicnessPoints;
				}
			}
		}
		TActorIterator<ASmallMilitaryBuilding> SmallMilitaryBuilding(GetWorld());
		for (SmallMilitaryBuilding; SmallMilitaryBuilding; ++SmallMilitaryBuilding)
		{
			int VictoryPoints = 50;
			int EpicnessPoints = 5;
			FUniqueNetIdRepl ID = GetIdBySide(SmallMilitaryBuilding->GetSide());

			for (int i(0); i < PlayersIDs.Num(); i++)
			{
				if (PlayersPoints[i].ID == ID)
				{
					PlayersPoints[i].VictoryPoints += VictoryPoints;
					PlayersPoints[i].EpicnessPoints += EpicnessPoints;
				}
			}
		}
		TActorIterator<AEconomicBuilding> EconomicBuilding(GetWorld());
		for (EconomicBuilding; EconomicBuilding; ++EconomicBuilding)
		{
			int VictoryPoints = 10;
			int EpicnessPoints = 1;
			FUniqueNetIdRepl ID = GetIdBySide(EconomicBuilding->GetSide());

			for (int i(0); i < PlayersIDs.Num(); i++)
			{
				if (PlayersPoints[i].ID == ID)
				{
					PlayersPoints[i].VictoryPoints += VictoryPoints;
					PlayersPoints[i].EpicnessPoints += EpicnessPoints;
				}
			}
		}
		TActorIterator<AUnit> Unit(GetWorld());
		for (Unit; Unit; ++Unit)
		{
			int Points = 1;
			FUniqueNetIdRepl ID = GetIdBySide(Unit->GetSide());

			for (int i(0); i < PlayersIDs.Num(); i++)
			{
				if (PlayersPoints[i].ID == ID)
				{
					PlayersPoints[i].VictoryPoints += Points;
					PlayersPoints[i].EpicnessPoints += Points;
				}
			}
		}

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
		{
			CurrentTime += DeltaTime;
			float Minutes = CurrentTime / 60.f;
			for (int i(0); i < PlayerArray.Num(); i++)
				Cast<AMultiplayerState>(PlayerArray[i])->GetSkillsTree()->SetPoints(FMath::RoundToNegativeInfinity(Minutes) + 2);
		}

		if (bGameEnded && CountDown < -1.f)
		{
			bGameActive = false;
			bGameSoonActive = false;

			AMultiplayerSinahMode* Mode = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode());
			Mode->EndMatch();

			Winner = GetIdBySide(FirstSide);
		}
	}
}

bool AMultiplayerGameState::IsGameActive()
{
	return bGameActive;
}

bool AMultiplayerGameState::IsGameSoonActive()
{
	return bGameSoonActive;
}

void AMultiplayerGameState::PreBeginGame()
{
	CountDown = 3.f;
	bGameSoonActive = true;

	for (int i(0); i < PlayerArray.Num(); i++)
	{
		TActorIterator<ABuilding> Building(GetWorld());
		for (Building; Building; ++Building)
		{
			ESide PlayerSide = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode())->GetPlayerSide(PlayerArray[i]->UniqueId);
			if (Building->GetSide() == PlayerSide && !IsValid(Building->GetPlayer()))
				Building->SetSide(PlayerSide, Cast<AMultiplayerState>(PlayerArray[i]));
		}
	}
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

TArray<FPointsForPlayerStruct> AMultiplayerGameState::GetPlayersPoints()
{
	return PlayersPoints;
}

FUniqueNetIdRepl AMultiplayerGameState::GetIdBySide(ESide Side)
{
	TArray<FUniqueNetIdRepl> PlayersIDs = Cast<AMultiplayerSinahMode>(GetWorld()->GetAuthGameMode())->GetPlayersIDs();
	if (Side == ESide::Blue && PlayersIDs.IsValidIndex(0))
		return PlayersIDs[0];
	else if (Side == ESide::Red && PlayersIDs.IsValidIndex(1))
		return PlayersIDs[1];
	else
		return nullptr;
}

int AMultiplayerGameState::GetTime()
{
	return CurrentTime;
}

FUniqueNetIdRepl AMultiplayerGameState::GetWinner()
{
	return Winner;
}
void AMultiplayerGameState::SetCivForPlayer(FUniqueNetIdRepl ID, ECivs Civ)
{
	if (Role == ROLE_Authority)
	{
		FCivForPlayerStruct Struct;
		Struct.ID = ID;
		Struct.Civ = Civ;
		PlayersCiv.Add(Struct);
	}
}
TArray<FCivForPlayerStruct> AMultiplayerGameState::GetAllCivs()
{
	return PlayersCiv;
}

//Replication
void AMultiplayerGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerGameState, bGameActive);
	DOREPLIFETIME(AMultiplayerGameState, bGameSoonActive);
	DOREPLIFETIME(AMultiplayerGameState, Winner);
	DOREPLIFETIME(AMultiplayerGameState, StateInfo);
	DOREPLIFETIME(AMultiplayerGameState, CurrentTime);

	DOREPLIFETIME(AMultiplayerGameState, PlayersPoints);
	DOREPLIFETIME(AMultiplayerGameState, PlayersCiv);
}