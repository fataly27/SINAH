// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "CristalsEconomicBuilding.h"

ACristalsEconomicBuilding::ACristalsEconomicBuilding() : Super()
{
	CostInFood = { 200, 400, 600 };
	CostInCells = { 600, 1000, 1400 };
	CostInMetal = { 400, 700, 1000 };
	CostInCristals = { 800, 1700, 2400 };
}