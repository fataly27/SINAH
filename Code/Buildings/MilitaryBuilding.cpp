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
void AMilitaryBuilding::ChangeDecals(bool bNormal)
{
	bool IsPlayer(Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController()) && Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->GetSide() == GetSide());
	bool Reverse(Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController()) && Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->IsOpponentViewEnabled());

	if ((IsPlayer && !Reverse) || (!IsPlayer && Reverse))
	{
		OpponentLifeZone->SetHiddenInGame(bNormal);
		OpponentSpeedZone->SetHiddenInGame(bNormal);

		if (MySide == ESide::Neutral)
		{
			PlayerLifeZone->SetHiddenInGame(bNormal);
			PlayerSpeedZone->SetHiddenInGame(bNormal);
		}
		else
		{
			PlayerLifeZone->SetHiddenInGame(!bNormal);
			PlayerSpeedZone->SetHiddenInGame(!bNormal);
		}
	}
	else
	{
		OpponentLifeZone->SetHiddenInGame(!bNormal);
		OpponentSpeedZone->SetHiddenInGame(!bNormal);
		PlayerLifeZone->SetHiddenInGame(bNormal);
		PlayerSpeedZone->SetHiddenInGame(bNormal);
	}
}

void AMilitaryBuilding::Multicast_LevelUpPlayerLifeZoneEffect_Implementation()
{
	PlayerLifeZone->LevelUpEffect();
}
void AMilitaryBuilding::Multicast_LevelUpOpponentLifeZoneEffect_Implementation()
{
	OpponentLifeZone->LevelUpEffect();
}
void AMilitaryBuilding::Multicast_LevelUpPlayerSpeedZoneEffect_Implementation()
{
	PlayerSpeedZone->LevelUpEffect();
}
void AMilitaryBuilding::Multicast_LevelUpOpponentSpeedZoneEffect_Implementation()
{
	OpponentSpeedZone->LevelUpEffect();
}

void AMilitaryBuilding::Multicast_LevelUpPlayerLifeZoneReach_Implementation()
{
	PlayerLifeZone->LevelUpReach();

	ChangeDecals(false);
	ChangeDecals(true);
}
void AMilitaryBuilding::Multicast_LevelUpOpponentLifeZoneReach_Implementation()
{
	OpponentLifeZone->LevelUpReach();

	ChangeDecals(false);
	ChangeDecals(true);
}
void AMilitaryBuilding::Multicast_LevelUpPlayerSpeedZoneReach_Implementation()
{
	PlayerSpeedZone->LevelUpReach();

	ChangeDecals(false);
	ChangeDecals(true);
}
void AMilitaryBuilding::Multicast_LevelUpOpponentSpeedZoneReach_Implementation()
{
	OpponentSpeedZone->LevelUpReach();

	ChangeDecals(false);
	ChangeDecals(true);
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

//Points
int AMilitaryBuilding::GetPoints()
{
	return Points;
}
void AMilitaryBuilding::AddPoints(unsigned int Add)
{
	if (Role == ROLE_Authority)
		Points += Add;
}
void AMilitaryBuilding::UseOnePoint()
{
	if (Role == ROLE_Authority)
		Points -= 1;
}

//Level
void AMilitaryBuilding::LevelUp()
{
	AddPoints(2);
	Super::LevelUp();
}

//Index
int AMilitaryBuilding::GetIndex()
{
	return MapIndex;
}

//ESide
void AMilitaryBuilding::SetSide(ESide NewSide)
{
	Super::SetSide(NewSide);

	Points = 2;

	Multicast_SetMilitarySide();
	Multicast_ChangeDecals();
}
void AMilitaryBuilding::Multicast_SetMilitarySide_Implementation()
{
	PlayerLifeZone->SetReachLevel(1);
	PlayerLifeZone->SetEffectLevel(0);
	PlayerSpeedZone->SetReachLevel(1);
	PlayerSpeedZone->SetEffectLevel(0);

	OpponentLifeZone->SetReachLevel(1);
	OpponentLifeZone->SetEffectLevel(0);
	OpponentSpeedZone->SetReachLevel(1);
	OpponentSpeedZone->SetEffectLevel(0);
}
void AMilitaryBuilding::Multicast_ChangeDecals_Implementation()
{
	ChangeDecals(false);
	ChangeDecals(true);
}

//Replication
void AMilitaryBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMilitaryBuilding, Points);
}