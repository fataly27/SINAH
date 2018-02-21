// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "../Units/Unit.h"
#include "ModesWidget.h"

void UModesWidget::SetModesVisibility(ESlateVisibility Unit)
{
	ModesVisibility = Unit;
}
void UModesWidget::SetAllButtonsEnabled(bool bAll)
{
	InvisibleButtonEnabled = bAll;
	OtherButtonsEnabled = bAll;
}
void UModesWidget::SetInvisibleButtonEnabled(bool bInvisible)
{
	InvisibleButtonEnabled = bInvisible;
}

void UModesWidget::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}