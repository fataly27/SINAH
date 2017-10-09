// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "../Units/Unit.h"
#include "ModesWidget.h"

void UModesWidget::SetModesVisibility(ESlateVisibility Unit)
{
	ModesVisibility = Unit;
}
void UModesWidget::SetAllButtonsEnabled(bool All)
{
	InvisibleButtonEnabled = All;
	OtherButtonsEnabled = All;
}
void UModesWidget::SetInvisibleButtonEnabled(bool Invisible)
{
	InvisibleButtonEnabled = Invisible;
}

void UModesWidget::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}