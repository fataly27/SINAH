// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "GameAfterEndingWidget.h"

void UGameAfterEndingWidget::SetDidWin(bool Win)
{
	if (Win)
		Text = "V I C T O R Y";
	else
		Text = "D E F E A T";
}