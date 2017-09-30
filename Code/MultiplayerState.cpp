// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MultiplayerState.h"

AMultiplayerState::AMultiplayerState()
{
	AmountOfFood = 0;
	AmountOfMetal = 0;
	AmountOfCells = 0;
	AmountOfCristals = 0;

	FoodChange = 0;
	MetalChange = 0;
	CellsChange = 0;
	CristalsChange = 0;

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
	if (Role == ROLE_Authority)
	{
		AmountOfFood = Food;

		if (AmountOfFood < 0)
			AmountOfFood = 0;
	}
}
void AMultiplayerState::SetAmountOfMetal(int Metal)
{
	if (Role == ROLE_Authority)
	{
		AmountOfMetal = Metal;

		if (AmountOfMetal < 0)
			AmountOfMetal = 0;
	}
}
void AMultiplayerState::SetAmountOfCells(int Cells)
{
	if (Role == ROLE_Authority)
	{
		AmountOfCells = Cells;

		if (AmountOfCells < 0)
			AmountOfCells = 0;
	}
}
void AMultiplayerState::SetAmountOfCristals(int Cristals)
{
	if (Role == ROLE_Authority)
	{
		AmountOfCristals = Cristals;

		if (AmountOfCristals < 0)
			AmountOfCristals = 0;
	}
}


int AMultiplayerState::GetFoodChange()
{
	return FoodChange;
}
int AMultiplayerState::GetMetalChange()
{
	return MetalChange;
}
int AMultiplayerState::GetCellsChange()
{
	return CellsChange;
}
int AMultiplayerState::GetCristalsChange()
{
	return CristalsChange;
}

void AMultiplayerState::SetFoodChange(int Food)
{
	if (Role == ROLE_Authority)
		FoodChange = Food;
}
void AMultiplayerState::SetMetalChange(int Metal)
{
	if (Role == ROLE_Authority && Metal >= 0)
		MetalChange = Metal;
}
void AMultiplayerState::SetCellsChange(int Cells)
{
	if (Role == ROLE_Authority && Cells >= 0)
		CellsChange = Cells;
}
void AMultiplayerState::SetCristalsChange(int Cristals)
{
	if (Role == ROLE_Authority && Cristals >= 0)
		CristalsChange = Cristals;
}

//Replication
void AMultiplayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerState, AmountOfFood);
	DOREPLIFETIME(AMultiplayerState, AmountOfMetal);
	DOREPLIFETIME(AMultiplayerState, AmountOfCells);
	DOREPLIFETIME(AMultiplayerState, AmountOfCristals);

	DOREPLIFETIME(AMultiplayerState, FoodChange);
	DOREPLIFETIME(AMultiplayerState, MetalChange);
	DOREPLIFETIME(AMultiplayerState, CellsChange);
	DOREPLIFETIME(AMultiplayerState, CristalsChange);
}