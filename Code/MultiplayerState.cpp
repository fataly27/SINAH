// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MultiplayerState.h"

AMultiplayerState::AMultiplayerState()
{
	AmountOfFood = 0;
	AmountOfMetal = 0;
	AmountOfCells = 0;
	AmountOfCristals = 0;

	bReplicates = true;
}

int AMultiplayerState::GetAmountOfFood()
{
	return AmountOfFood;
}
int AMultiplayerState::GetAmountOfMetal()
{
	return AmountOfMetal;
}
int AMultiplayerState::GetAmountOfCells()
{
	return AmountOfCells;
}
int AMultiplayerState::GetAmountOfCristals()
{
	return AmountOfCristals;
}

void AMultiplayerState::SetAmountOfFood(int Food)
{
	AmountOfFood = Food;

	if (AmountOfFood < 0)
		AmountOfFood = 0;
}
void AMultiplayerState::SetAmountOfMetal(int Metal)
{
	AmountOfMetal = Metal;

	if (AmountOfMetal < 0)
		AmountOfMetal = 0;
}
void AMultiplayerState::SetAmountOfCells(int Cells)
{
	AmountOfCells = Cells;

	if (AmountOfCells < 0)
		AmountOfCells = 0;
}
void AMultiplayerState::SetAmountOfCristals(int Cristals)
{
	AmountOfCristals = Cristals;

	if (AmountOfCristals < 0)
		AmountOfCristals = 0;
}

//Replication
void AMultiplayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerState, AmountOfFood);
	DOREPLIFETIME(AMultiplayerState, AmountOfMetal);
	DOREPLIFETIME(AMultiplayerState, AmountOfCells);
	DOREPLIFETIME(AMultiplayerState, AmountOfCristals);
}