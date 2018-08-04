// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SkillButton.h"


void USkillButton::SetIndex(int Idx)
{
	Index = Idx;
	OnClicked.AddDynamic(this, &USkillButton::ClickFunction);
}
void USkillButton::ClickFunction()
{
	OnSkillSelected.Broadcast(Index);
}