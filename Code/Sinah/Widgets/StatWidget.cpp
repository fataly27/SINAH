// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "StatWidget.h"

void UStatWidget::SetColor(FLinearColor NewColor)
{
	Color = NewColor;
}
void UStatWidget::SetPVs(int Current, int Max)
{
	if (Current == 0 || Max == 0)
	{
		PVs = "";
		PVsVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		PVs = FString::FromInt(Current).Append("/").Append(FString::FromInt(Max));
		PVsVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetHeal(int NewHeal)
{
	if (NewHeal == 0)
	{
		Heal = "";
		HealVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		Heal = FString::FromInt(NewHeal).Append(" /s");
		HealVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetTheAttack(int Physic, int Magic)
{
	if (Physic == 0 && Magic == 0)
	{
		TheAttack = "";
		TheAttackVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		TheAttack = FString::FromInt(Physic).Append(" / ").Append(FString::FromInt(Magic));
		TheAttackVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetDefense(int Physic, int Magic)
{
	if (Physic == 0 && Magic == 0)
	{
		Defense = "";
		DefenseVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		Defense = FString::FromInt(Physic).Append(" / ").Append(FString::FromInt(Magic));
		DefenseVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetSpeed(float NewSpeed)
{
	if (NewSpeed == 0)
	{
		Speed = "";
		SpeedVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		FString DecimalPart = FString::FromInt((int)(NewSpeed * 10) % 10);
		Speed = FString::FromInt(NewSpeed).Append(".").Append(DecimalPart).Append(" m/s");
		SpeedVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetFieldOfSight(int NewFieldOfSight)
{
	if (NewFieldOfSight == 0)
	{
		FieldOfSight = "";
		FieldOfSightVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		FieldOfSight = FString::FromInt(NewFieldOfSight).Append(" m");
		FieldOfSightVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetRange(int NewRange)
{
	if (NewRange == 0)
	{
		Range = "";
		RangeVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		Range = FString::FromInt(NewRange).Append(" m");
		RangeVisibility = ESlateVisibility::Visible;
	}
}
void UStatWidget::SetFoodEaten(int NewFoodEaten)
{
	if (NewFoodEaten == 0)
	{
		FoodEaten = "";
		FoodEatenVisibility = ESlateVisibility::Hidden;
	}
	else
	{
		FoodEaten = FString::FromInt(NewFoodEaten).Append(" /s");
		FoodEatenVisibility = ESlateVisibility::Visible;
	}
}

void UStatWidget::SetStatsVisibility(ESlateVisibility Unit, ESlateVisibility Building)
{
	UnitStats = Unit;
	BuildingStats = Building;
}

void UStatWidget::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}