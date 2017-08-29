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
	Super::BeginPlay();

	PlayerLifeZone->Init(true);
	PlayerSpeedZone->Init(true);
	OpponentLifeZone->Init(false);
	OpponentSpeedZone->Init(false);

	ChangeDecals(false);
}

//Zones
void AMilitaryBuilding::ChangeDecals(bool Reverse)
{
	bool IsPlayer(Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->GetSide() == GetSide());

	if ((IsPlayer && !Reverse) || (!IsPlayer && Reverse))
	{
		PlayerLifeZone->SetHiddenInGame(false);
		PlayerSpeedZone->SetHiddenInGame(false);
		OpponentLifeZone->SetHiddenInGame(true);
		OpponentSpeedZone->SetHiddenInGame(true);
	}
	else
	{
		PlayerLifeZone->SetHiddenInGame(true);
		PlayerSpeedZone->SetHiddenInGame(true);
		OpponentLifeZone->SetHiddenInGame(false);
		OpponentSpeedZone->SetHiddenInGame(false);
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
		PlayerLifeZone->Multicast_SetReachLevel(1);
		PlayerLifeZone->Multicast_SetEffectLevel(0);
		PlayerSpeedZone->Multicast_SetReachLevel(1);
		PlayerSpeedZone->Multicast_SetEffectLevel(0);

		OpponentLifeZone->Multicast_SetReachLevel(1);
		OpponentLifeZone->Multicast_SetEffectLevel(0);
		OpponentSpeedZone->Multicast_SetReachLevel(1);
		OpponentSpeedZone->Multicast_SetEffectLevel(0);
	}
}


//Replication
void AMilitaryBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AMilitaryBuilding, PlayerLifeZone);
	DOREPLIFETIME(AMilitaryBuilding, OpponentLifeZone);
	DOREPLIFETIME(AMilitaryBuilding, PlayerSpeedZone);
	DOREPLIFETIME(AMilitaryBuilding, OpponentSpeedZone);
}