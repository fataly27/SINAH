// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "FoodEconomicBuilding.h"


AFoodEconomicBuilding::AFoodEconomicBuilding() : Super()
{
	COST_IN_FOOD = { 800, 1700, 2400 };
	COST_IN_CELLS = { 600, 1000, 1400 };
	COST_IN_METAL = { 400, 700, 1000 };
	COST_IN_CRISTALS = { 200, 400, 600 };
}