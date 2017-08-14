// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MetalEconomicBuilding.h"


AMetalEconomicBuilding::AMetalEconomicBuilding() : Super()
{
	COST_IN_FOOD = { 200, 400, 600 };
	COST_IN_CELLS = { 600, 1000, 1400 };
	COST_IN_METAL = { 1000, 2000, 3000 };
	COST_IN_CRISTALS = { 200, 400, 600 };
}