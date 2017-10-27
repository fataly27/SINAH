// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "LevelEconomicWidget.h"


void ULevelEconomicWidget::SetOutput(int Current, int Next)
{
	CurrentOutput = FString::FromInt(Current).Append(" / s");
	OutputAfterUpgrade = FString::FromInt(Next).Append(" / s");
}
void ULevelEconomicWidget::SetIsPlundered(bool Plundered)
{
	IsPlundered = Plundered;
}