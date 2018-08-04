// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "../SkillsTree.h"
#include "../MousePlayerController.h"
#include "GameBeforeStartingWidget.h"


void UGameBeforeStartingWidget::LaunchGame()
{
	GameLaunched = true;
	SelectSkill(-1);
	SendSkillsUpdate();
}
int UGameBeforeStartingWidget::HasGameBeenLaunched()
{
	return GameLaunched;
}

void UGameBeforeStartingWidget::AddOnePoint()
{
	Points++;
}
void UGameBeforeStartingWidget::SpendOnePoint()
{
	if (IsValid(CurrentSkill))
	{
		if (GameLaunched)
			Points--;

		if (Points < 0)
			Points = 0;
		else
		{
			CurrentSkill->Skill = true;
			if(GameLaunched)
				SendSkillsUpdate();
		}
	}
}
void UGameBeforeStartingWidget::SelectSkill(int Index)
{
	if (Skills.IsValidIndex(0))
	{
		if (Index == -1)
			CurrentSkill = nullptr;
		else
			CurrentSkill = Skills[Index];
	}
}
void UGameBeforeStartingWidget::SetSkillTree(ASkillsTree* Tree)
{
	if (Tree && (GameLaunched || Skills.Num() == 0))
	{
		int j(0);
		for (int i(0); i < Tree->GetSkills().Num(); i++)
		{
			if (IsValid(Tree->GetSkills()[i]))
				j++;
		}

		if (j == Tree->GetSkills().Num())
		{
			Skills = Tree->GetSkills();

			if (GameLaunched)
				Points = Tree->SetSkills(Skills);
			else
				Points = 0;
		}
	}
}
bool UGameBeforeStartingWidget::IsSkillEnabled(int Index)
{
	if (Skills.IsValidIndex(Index))
		return Skills[Index] && Skills[Index]->Skill;
	else
		return false;
}
void UGameBeforeStartingWidget::ClearSkills()
{
	for (int i(0); i < Skills.Num(); i++)
	{
		Skills[i]->Skill = false;
	}
}
void UGameBeforeStartingWidget::SendSkillsUpdate()
{
	Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->UpdateSkillTree(Skills);
}