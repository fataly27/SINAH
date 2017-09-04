// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Zones/SpeedZone.h"
#include "Zones/LifeZone.h"
#include "../MousePlayerController.h"
#include "MilitaryBuilding.h"


AMilitaryBuilding::AMilitaryBuilding() : Super()
{
	PlayerLifeZone = CreateDefaultSubobject<ULifeZone>(TEXT("PlayerLifeZone"));
	PlayerSpeedZone = CreateDefaultSubobject<USpeedZone>(TEXT("PlayerSpeedZone"));
	OpponentLifeZone = CreateDefaultSubobject<ULifeZone>(TEXT("OpponentLifeZone"));
	OpponentSpeedZone = CreateDefaultSubobject<USpeedZone>(TEXT("OpponentSpeedZone"));

	PlayerSpeedZone->SetupAttachment(RootComponent);
	PlayerLifeZone->SetupAttachment(RootComponent);
	OpponentSpeedZone->SetupAttachment(RootComponent);
	OpponentLifeZone->SetupAttachment(RootComponent);

	PlayerLifeZone->SetIsReplicated(true);
	PlayerSpeedZone->SetIsReplicated(true);
	OpponentLifeZone->SetIsReplicated(true);
	OpponentSpeedZone->SetIsReplicated(true);
}

void AMilitaryBuilding::BeginPlay()
{
	PlayerLifeZone->Init(true);
	PlayerSpeedZone->Init(true);
	OpponentLifeZone->Init(false);
	OpponentSpeedZone->Init(false);

	Super::BeginPlay();

	ChangeDecals();
}

//Zones
void AMilitaryBuilding::ChangeDecals()
{
	bool IsPlayer(Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->GetSide() == GetSide());
	bool Reverse(Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->IsOpponentViewEnabled());

	if ((IsPlayer && !Reverse) || (!IsPlayer && Reverse))
	{
		OpponentLifeZone->SetHiddenInGame(true);
		OpponentSpeedZone->SetHiddenInGame(true);

		if (MySide == Side::Neutral)
		{
			PlayerLifeZone->SetHiddenInGame(true);
			PlayerSpeedZone->SetHiddenInGame(true);
		}
		else
		{
			PlayerLifeZone->SetHiddenInGame(false);
			PlayerSpeedZone->SetHiddenInGame(false);
		}
	}
	else
	{
		OpponentLifeZone->SetHiddenInGame(false);
		OpponentSpeedZone->SetHiddenInGame(false);
		PlayerLifeZone->SetHiddenInGame(true);
		PlayerSpeedZone->SetHiddenInGame(true);
	}
}

ULifeZone* AMilitaryBuilding::GetPlayerLifeZone()
{
	return PlayerLifeZone;
}
ULifeZone* AMilitaryBuilding::GetOpponentLifeZone()
{
	return OpponentLifeZone;
}
USpeedZone* AMilitaryBuilding::GetPlayerSpeedZone()
{
	return PlayerSpeedZone;
}
USpeedZone* AMilitaryBuilding::GetOpponentSpeedZone()
{
	return OpponentSpeedZone;
}

//Side
void AMilitaryBuilding::SetSide(Side NewSide)
{
	Super::SetSide(NewSide);

	if (Role == ROLE_Authority)
	{
		PlayerLifeZone->SetReachLevel(5);
		PlayerLifeZone->SetEffectLevel(4);
		PlayerSpeedZone->SetReachLevel(5);
		PlayerSpeedZone->SetEffectLevel(4);

		OpponentLifeZone->SetReachLevel(5);
		OpponentLifeZone->SetEffectLevel(4);
		OpponentSpeedZone->SetReachLevel(5);
		OpponentSpeedZone->SetEffectLevel(4);
	}
	
	Multicast_ChangeDecals();
}
void AMilitaryBuilding::Multicast_ChangeDecals_Implementation()
{
	ChangeDecals();
}

//Replication
void AMilitaryBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AMilitaryBuilding, PlayerLifeZone);
	DOREPLIFETIME(AMilitaryBuilding, OpponentLifeZone);
	DOREPLIFETIME(AMilitaryBuilding, PlayerSpeedZone);
	DOREPLIFETIME(AMilitaryBuilding, OpponentSpeedZone);
}