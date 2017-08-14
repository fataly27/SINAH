// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "CellsEconomicBuilding.h"


ACellsEconomicBuilding::ACellsEconomicBuilding() : Super()
{
	COST_IN_FOOD = { 200, 400, 600 };
	COST_IN_CELLS = { 1200, 2300, 3400 };
	COST_IN_METAL = { 400, 700, 1000 };
	COST_IN_CRISTALS = { 200, 400, 600 };
}