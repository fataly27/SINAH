// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "CellsEconomicBuilding.h"


ACellsEconomicBuilding::ACellsEconomicBuilding() : Super()
{
	CostInFood = { 400, 800, 1600 };
	CostInCells = { 100, 200, 400 };
	CostInMetal = { 300, 600, 1200 };
	CostInCristals = { 500, 1000, 2000 };
}