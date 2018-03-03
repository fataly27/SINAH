// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "UpWidget.h"

void UUpWidget::SetFood(int Food)
{
	if (Food >= 0)
		AmountOfFood = FString::FromInt(Food);
	else
		AmountOfFood = "0";
}
void UUpWidget::SetMetal(int Metal)
{
	if (Metal >= 0)
		AmountOfMetal = FString::FromInt(Metal);
	else
		AmountOfMetal = "0";
}
void UUpWidget::SetCells(int Cells)
{
	if (Cells >= 0)
		AmountOfCells = FString::FromInt(Cells);
	else
		AmountOfCells = "0";
}
void UUpWidget::SetCristals(int Cristals)
{
	if (Cristals >= 0)
		AmountOfCristals = FString::FromInt(Cristals);
	else
		AmountOfCristals = "0";
}

void UUpWidget::SetFoodChange(int Food)
{
	if (Food < 0)
		FoodChange = FString::FromInt(Food);
	else
		FoodChange = FString("+").Append(FString::FromInt(Food));
}
void UUpWidget::SetMetalChange(int Metal)
{
	MetalChange = FString("+").Append(FString::FromInt(Metal));
}
void UUpWidget::SetCellsChange(int Cells)
{
	CellsChange = FString("+").Append(FString::FromInt(Cells));
}
void UUpWidget::SetCristalsChange(int Cristals)
{
	CristalsChange = FString("+").Append(FString::FromInt(Cristals));
}

void UUpWidget::SetTime(int BaseSeconds)
{
	int Minutes = BaseSeconds / 60;
	int Seconds = BaseSeconds % 60;

	FString MinutesString;
	FString SecondsString;

	if (Minutes < 10)
		MinutesString = "0" + FString::FromInt(Minutes) + ":";
	else
		MinutesString = FString::FromInt(Minutes) + ":";

	if (Seconds < 10)
		SecondsString = "0" + FString::FromInt(Seconds);
	else
		SecondsString = FString::FromInt(Seconds);

	TheTimer = MinutesString.Append(SecondsString);
}