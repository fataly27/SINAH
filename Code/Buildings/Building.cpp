// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Building.h"


// Sets default values
ABuilding::ABuilding() : IsVisibleForOpponent(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Selection
void ABuilding::Select()
{
	Selected = true;
	SelectionMark->SetHiddenInGame(false);
}
void ABuilding::Unselect()
{
	Selected = false;
	SelectionMark->SetHiddenInGame(true);
}
bool ABuilding::IsSelected()
{
	return Selected;
}

//Color
void ABuilding::AmIBlue(bool color)
{
	ImBlue = color;

	if (ImBlue)
		SelectionMark->SetMaterial(0, BlueCircle);
	else
		SelectionMark->SetMaterial(0, RedCircle);
}
bool ABuilding::TellIfImBlue()
{
	return ImBlue;
}

//Attack
void ABuilding::ReceiveDamages(float Physic, float Magic)
{

}

//Dying
bool ABuilding::IsPendingKill()
{
	return IsPendingKillPending();
}

//Statistics Getters
int ABuilding::GetMaxLife()
{
	return ActualMaxLife;
}
int ABuilding::GetCurrentLife()
{
	return CurrentLife;
}
float ABuilding::GetFieldOfSight()
{
	return ActualFieldOfSight;
}
float ABuilding::GetHalfHeight()
{
	return 50.f;
}
float ABuilding::GetHeal()
{
	return ActualHeal;
}

//Visibility
bool ABuilding::GetOpponentVisibility()
{
	return IsVisibleForOpponent;
}
FVector ABuilding::GetLocation()
{
	return GetActorLocation();
}

//Replication
void ABuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(ABuilding, CurrentLife);
	DOREPLIFETIME(ABuilding, ActualMaxLife);
	DOREPLIFETIME(ABuilding, ActualFieldOfSight);
	DOREPLIFETIME(ABuilding, ActualHeal);

	DOREPLIFETIME(ABuilding, IsVisibleForOpponent);
}