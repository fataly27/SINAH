// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "LevelMilitaryWidget.h"

void ULevelMilitaryWidget::SetPoints(int Pts)
{
	Points = Pts;
	String_Points = FString::FromInt(Pts);
}

void ULevelMilitaryWidget::SetPlayerLifeEffect(int Current, int Max)
{
	CurrentPlayerLifeEffect = FString::FromInt(Current);
	MaxPlayerLifeEffect = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bPlayerLifePossibleEffect = true;
	else
		bPlayerLifePossibleEffect = false;
}
void ULevelMilitaryWidget::SetPlayerSpeedEffect(int Current, int Max)
{
	CurrentPlayerSpeedEffect = FString::FromInt(Current);
	MaxPlayerSpeedEffect = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bPlayerSpeedPossibleEffect = true;
	else
		bPlayerSpeedPossibleEffect = false;
}
void ULevelMilitaryWidget::SetOpponentLifeEffect(int Current, int Max)
{
	CurrentOpponentLifeEffect = FString::FromInt(Current);
	MaxOpponentLifeEffect = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bOpponentLifePossibleEffect = true;
	else
		bOpponentLifePossibleEffect = false;
}
void ULevelMilitaryWidget::SetOpponentSpeedEffect(int Current, int Max)
{
	CurrentOpponentSpeedEffect = FString::FromInt(Current);
	MaxOpponentSpeedEffect = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bOpponentSpeedPossibleEffect = true;
	else
		bOpponentSpeedPossibleEffect = false;
}

void ULevelMilitaryWidget::SetPlayerLifeReach(int Current, int Max)
{
	CurrentPlayerLifeReach = FString::FromInt(Current);
	MaxPlayerLifeReach = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bPlayerLifePossibleReach = true;
	else
		bPlayerLifePossibleReach = false;
}
void ULevelMilitaryWidget::SetPlayerSpeedReach(int Current, int Max)
{
	CurrentPlayerSpeedReach = FString::FromInt(Current);
	MaxPlayerSpeedReach = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bPlayerSpeedPossibleReach = true;
	else
		bPlayerSpeedPossibleReach = false;
}
void ULevelMilitaryWidget::SetOpponentLifeReach(int Current, int Max)
{
	CurrentOpponentLifeReach = FString::FromInt(Current);
	MaxOpponentLifeReach = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bOpponentLifePossibleReach = true;
	else
		bOpponentLifePossibleReach = false;
}
void ULevelMilitaryWidget::SetOpponentSpeedReach(int Current, int Max)
{
	CurrentOpponentSpeedReach = FString::FromInt(Current);
	MaxOpponentSpeedReach = FString::FromInt(Max);

	if (Current < Max && Points > 0)
		bOpponentSpeedPossibleReach = true;
	else
		bOpponentSpeedPossibleReach = false;
}