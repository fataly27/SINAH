// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "LevelWidget.h"

void ULevelWidget::SetLevel(int CurrentLevel, int MaxLevel, FLinearColor NewColor)
{
	Level = FString("Level ").Append(FString::FromInt(CurrentLevel)).Append(" / ").Append(FString::FromInt(MaxLevel));
	Color = NewColor;
}
void ULevelWidget::SetRessourcesRequired(int Food, int Cells, int Metal, int Cristals, bool bFood, bool bCells, bool bMetal, bool bCristals)
{
	FoodRequired = FString::FromInt(Food);
	CellsRequired = FString::FromInt(Cells);
	MetalRequired = FString::FromInt(Metal);
	CristalsRequired = FString::FromInt(Cristals);

	IsThereEnoughFood = bFood;
	IsThereEnoughCells = bCells;
	IsThereEnoughMetal = bMetal;
	IsThereEnoughCristals = bCristals;
}
void ULevelWidget::SetIsLevelUpPossible(bool bPossible)
{
	IsLevelUpPossible = bPossible;
}
void ULevelWidget::SetIsLevelVisible(bool bVisible)
{
	if (bVisible)
		IsLevelVisible = ESlateVisibility::SelfHitTestInvisible;
	else
		IsLevelVisible = ESlateVisibility::Hidden;
}
void ULevelWidget::SetAreDetailsVisible(bool bVisible)
{
	if (bVisible)
		AreDetailsVisible = ESlateVisibility::SelfHitTestInvisible;
	else
		AreDetailsVisible = ESlateVisibility::Hidden;
}
void ULevelWidget::SetIsLevelUpVisible(bool bVisible)
{
	if (bVisible)
		IsLevelUpVisible = ESlateVisibility::SelfHitTestInvisible;
	else
		IsLevelUpVisible = ESlateVisibility::Hidden;
}