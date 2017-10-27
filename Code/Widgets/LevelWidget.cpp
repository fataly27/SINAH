// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "LevelWidget.h"

void ULevelWidget::SetLevel(int CurrentLevel, int MaxLevel, FLinearColor NewColor)
{
	Level = FString("Level ").Append(FString::FromInt(CurrentLevel)).Append(" / ").Append(FString::FromInt(MaxLevel));
	Color = NewColor;
}
void ULevelWidget::SetRessourcesRequired(int Food, int Cells, int Metal, int Cristals, bool IsFood, bool IsCells, bool IsMetal, bool IsCristals)
{
	FoodRequired = FString::FromInt(Food);
	CellsRequired = FString::FromInt(Cells);
	MetalRequired = FString::FromInt(Metal);
	CristalsRequired = FString::FromInt(Cristals);

	IsThereEnoughFood = IsFood;
	IsThereEnoughCells = IsCells;
	IsThereEnoughMetal = IsMetal;
	IsThereEnoughCristals = IsCristals;
}
void ULevelWidget::SetIsLevelUpPossible(bool Possible)
{
	IsLevelUpPossible = Possible;
}
void ULevelWidget::SetIsLevelVisible(bool Visible)
{
	if (Visible)
		IsLevelVisible = ESlateVisibility::SelfHitTestInvisible;
	else
		IsLevelVisible = ESlateVisibility::Hidden;
}
void ULevelWidget::SetAreDetailsVisible(bool Visible)
{
	if (Visible)
		AreDetailsVisible = ESlateVisibility::SelfHitTestInvisible;
	else
		AreDetailsVisible = ESlateVisibility::Hidden;
}
void ULevelWidget::SetIsLevelUpVisible(bool Visible)
{
	if (Visible)
		IsLevelUpVisible = ESlateVisibility::SelfHitTestInvisible;
	else
		IsLevelUpVisible = ESlateVisibility::Hidden;
}