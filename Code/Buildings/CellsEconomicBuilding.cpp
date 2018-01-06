// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "CellsEconomicBuilding.h"


ACellsEconomicBuilding::ACellsEconomicBuilding() : Super()
{
	CostInFood = { 200, 400, 600 };
	CostInCells = { 1200, 2300, 3400 };
	CostInMetal = { 400, 700, 1000 };
	CostInCristals = { 200, 400, 600 };
}