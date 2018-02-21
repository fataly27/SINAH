// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "CristalsEconomicBuilding.h"

ACristalsEconomicBuilding::ACristalsEconomicBuilding() : Super()
{
	CostInFood = { 500, 1000, 2000 };
	CostInCells = { 300, 600, 1200 };
	CostInMetal = { 400, 800, 1600 };
	CostInCristals = { 100, 200, 400 };
}