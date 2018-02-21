// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "FoodEconomicBuilding.h"


AFoodEconomicBuilding::AFoodEconomicBuilding() : Super()
{
	CostInFood = { 100, 200, 400 };
	CostInCells = { 400, 800, 1600 };
	CostInMetal = { 500, 1000, 2000 };
	CostInCristals = { 300, 600, 1200 };
}