// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MetalEconomicBuilding.h"


AMetalEconomicBuilding::AMetalEconomicBuilding() : Super()
{
	CostInFood = { 200, 400, 600 };
	CostInCells = { 600, 1000, 1400 };
	CostInMetal = { 1000, 2000, 3000 };
	CostInCristals = { 200, 400, 600 };
}