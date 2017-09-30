// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "StatWidget.h"

void UStatWidget::SetTitle(FString NewTitle, FLinearColor NewColor)
{
	Title = NewTitle;
	Color = NewColor;
}
void UStatWidget::SetPVs(int Current, int Max)
{
	if (Current == 0 || Max == 0)
		PVs = "";
	else
		PVs = FString("PVs : ").Append(FString::FromInt(Current)).Append("/").Append(FString::FromInt(Max));
}
void UStatWidget::SetHeal(int NewHeal)
{
	if (NewHeal == 0)
		Heal = "";
	else
		Heal = FString("Heal : ").Append(FString::FromInt(NewHeal)).Append("/s");
}
void UStatWidget::SetTheAttack(int Physic, int Magic)
{
	if (Physic == 0 && Magic == 0)
		TheAttack = "";
	else
		TheAttack = FString("Attack : ").Append(FString::FromInt(Physic)).Append(" (physic), ").Append(FString::FromInt(Magic)).Append(" (magic)");
}
void UStatWidget::SetDefense(int Physic, int Magic)
{
	if (Physic == 0 && Magic == 0)
		Defense = "";
	else
		Defense = FString("Defense : ").Append(FString::FromInt(Physic)).Append(" (physic), ").Append(FString::FromInt(Magic)).Append(" (magic)");
}
void UStatWidget::SetSpeed(int NewSpeed)
{
	if (NewSpeed == 0)
		Speed = "";
	else
		Speed = FString("Speed : ").Append(FString::FromInt(NewSpeed)).Append("m/s");
}
void UStatWidget::SetFieldOfSight(int NewFieldOfSight)
{
	if (NewFieldOfSight == 0)
		FieldOfSight = "";
	else
		FieldOfSight = FString("Field Of Sight : ").Append(FString::FromInt(NewFieldOfSight)).Append("m");
}
void UStatWidget::SetRange(int NewRange)
{
	if (NewRange == 0)
		Range = "";
	else
		Range = FString("Range : ").Append(FString::FromInt(NewRange)).Append("m");
}
void UStatWidget::SetFoodEaten(int NewFoodEaten)
{
	if (NewFoodEaten == 0)
		FoodEaten = "";
	else
		FoodEaten = FString("Food Eaten : ").Append(FString::FromInt(NewFoodEaten)).Append("/s");
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