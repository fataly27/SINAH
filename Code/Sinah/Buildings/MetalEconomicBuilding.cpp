// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MetalEconomicBuilding.h"


AMetalEconomicBuilding::AMetalEconomicBuilding() : Super()
{
	CostInFood = { 300, 600, 1200 };
	CostInCells = { 500, 1000, 2000 };
	CostInMetal = { 100, 200, 400 };
	CostInCristals = { 400, 800, 1600 };
}