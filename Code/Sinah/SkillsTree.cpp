// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SkillsTree.h"

ASkill::ASkill() : Super()
{
	bReplicates = true;
	bAlwaysRelevant = true;

	Skill = false;
	Parent = nullptr;
	Title = FText();
	Description = FText();
}

//Replication
void ASkill::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkill, Skill);
	DOREPLIFETIME(ASkill, Parent);
	DOREPLIFETIME(ASkill, Title);
	DOREPLIFETIME(ASkill, Description);
}

// -------------------------------------------------------

ASkillsTree::ASkillsTree() : Super()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}
void ASkillsTree::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		for (int i(0); i < 39; i++)
		{
			Skills.Add(GetWorld()->SpawnActor<ASkill>());
		}

		for (int i(0); i < 12; i++)
		{
			FString TitleBase;
			FString DescriptionBase;

			switch (i)
			{
				case 0:
					TitleBase = FString("Physic Attack ");
					DescriptionBase = FString("Increase physic attack by ");
					break;

				case 1:
					TitleBase = FString("Magic Attack ");
					DescriptionBase = FString("Increase magic attack by ");
					break;

				case 2:
					TitleBase = FString("Physic Defense ");
					DescriptionBase = FString("Increase physic defense by ");
					break;

				case 3:
					TitleBase = FString("Magic Defense ");
					DescriptionBase = FString("Increase magic defense by ");
					break;

				case 4:
					TitleBase = FString("Building Sight ");
					DescriptionBase = FString("Increase building sight by ");
					break;

				case 5:
					TitleBase = FString("Unit Sight ");
					DescriptionBase = FString("Increase unit sight by ");
					break;

				case 6:
					TitleBase = FString("Ressources ");
					DescriptionBase = FString("Increase ressources gathering by ");
					break;

				case 7:
					TitleBase = FString("Plundering ");
					DescriptionBase = FString("Increase ressources gathered").Append(LINE_TERMINATOR).Append("during plundering by ");
					break;

				case 8:
					TitleBase = FString("Building Conversion ");
					DescriptionBase = FString("Increase chances to keep").Append(LINE_TERMINATOR).Append("a higher level when conquering a building to").Append(LINE_TERMINATOR);
					break;

				case 9:
					TitleBase = FString("Mode Change ");
					DescriptionBase = FString("Decrease time taken by units").Append(LINE_TERMINATOR).Append("to change mode by ");
					break;

				case 10:
					TitleBase = FString("Building Heal ");
					DescriptionBase = FString("Increase amount of heal received").Append(LINE_TERMINATOR).Append("by buildings by ");
					break;

				case 11:
					TitleBase = FString("Unit Spawn Life ");
					DescriptionBase = FString("Increase part of maximum life").Append(LINE_TERMINATOR).Append("received by units at spawn to ");
					break;
			}

			for (int j(0); j < 3; j++)
			{
				if (j != 0)
					Skills[3 * i + j]->Parent = Skills[3 * i + j - 1];

				FString Title = TitleBase;
				FString Description = DescriptionBase;

				switch (j)
				{
					case 0:
						Title.Append("I");
						if (i == 6)
							Description.Append("10%");
						else if (i == 8)
							Description.Append("100% aiming at level 2");
						else if (i == 11)
							Description.Append("40%");
						else
							Description.Append("20%");
						break;

					case 1:
						Title.Append("II");
						if (i == 6)
							Description.Append("20%");
						else if (i == 8)
							Description.Append("50% aiming at level 3");
						else if (i == 11)
							Description.Append("60%");
						else
							Description.Append("40%");
						break;

					case 2:
						Title.Append("III");
						if (i == 6)
							Description.Append("30%");
						else if (i == 8)
							Description.Append("100% aiming at level 3");
						else if (i == 11)
							Description.Append("80%");
						else
							Description.Append("60%");
						break;
				}
				Skills[3 * i + j]->Title = FText::FromString(Title);
				Skills[3 * i + j]->Description = FText::FromString(Description);
			}
		}

		Skills[36]->Parent = nullptr;
		Skills[36]->Title = FText::FromString("Invisibility");
		Skills[36]->Description = FText::FromString("Enable units to use invisiblity mode");

		Skills[37]->Parent = Skills[36];
		Skills[37]->Title = FText::FromString("Invisibility Time");
		Skills[37]->Description = FText::FromString("Increase invisibility duration by 40%");

		Skills[38]->Parent = Skills[36];
		Skills[38]->Title = FText::FromString("Invisibility Cooldown");
		Skills[38]->Description = FText::FromString(FString("Decrease time required to reuse").Append(LINE_TERMINATOR).Append("invisibility mode by 40%"));
	}
}

float ASkillsTree::GetPhysicAttackModifier()
{
	float PhysicAttackModifier = 1.f;

	if (Skills[0] && Skills[0]->Skill)
	{
		PhysicAttackModifier = 1.2f;
		if (Skills[1] && Skills[1]->Skill)
		{
			PhysicAttackModifier = 1.4f;
			if (Skills[2] && Skills[2]->Skill)
				PhysicAttackModifier = 1.6f;
		}
	}

	return PhysicAttackModifier;
}
float ASkillsTree::GetMagicAttackModifier()
{
	float MagicAttackModifier = 1.f;

	if (Skills[3] && Skills[3]->Skill)
	{
		MagicAttackModifier = 1.2f;
		if (Skills[4] && Skills[4]->Skill)
		{
			MagicAttackModifier = 1.4f;
			if (Skills[5] && Skills[5]->Skill)
				MagicAttackModifier = 1.6f;
		}
	}

	return MagicAttackModifier;

}
float ASkillsTree::GetPhysicDefenseModifier()
{
	float PhysicDefenseModifier = 1.f;

	if (Skills[6] && Skills[6]->Skill)
	{
		PhysicDefenseModifier = 1.2f;
		if (Skills[7] && Skills[7]->Skill)
		{
			PhysicDefenseModifier = 1.4f;
			if (Skills[8] && Skills[8]->Skill)
				PhysicDefenseModifier = 1.6f;
		}
	}

	return PhysicDefenseModifier;
}
float ASkillsTree::GetMagicDefenseModifier()
{
	float MagicDefenseModifier = 1.f;

	if (Skills[9] && Skills[9]->Skill)
	{
		MagicDefenseModifier = 1.2f;
		if (Skills[10] && Skills[10]->Skill)
		{
			MagicDefenseModifier = 1.4f;
			if (Skills[11] && Skills[11]->Skill)
				MagicDefenseModifier = 1.6f;
		}
	}

	return MagicDefenseModifier;
}

float ASkillsTree::GetBuildingSightModifier()
{
	float BuildingSightModifier = 1.f;

	if (Skills[12] && Skills[12]->Skill)
	{
		BuildingSightModifier = 1.2f;
		if (Skills[13] && Skills[13]->Skill)
		{
			BuildingSightModifier = 1.4f;
			if (Skills[14] && Skills[14]->Skill)
				BuildingSightModifier = 1.6f;
		}
	}

	return BuildingSightModifier;
}
float ASkillsTree::GetUnitSightModifier()
{
	float UnitSightModifier = 1.f;

	if (Skills[15] && Skills[15]->Skill)
	{
		UnitSightModifier = 1.2f;
		if (Skills[16] && Skills[16]->Skill)
		{
			UnitSightModifier = 1.4f;
			if (Skills[17] && Skills[17]->Skill)
				UnitSightModifier = 1.6f;
		}
	}

	return UnitSightModifier;
}

float ASkillsTree::GetRessourcesModifier()
{
	float RessourcesModifier = 1.f;

	if (Skills[18] && Skills[18]->Skill)
	{
		RessourcesModifier = 1.1f;
		if (Skills[19] && Skills[19]->Skill)
		{
			RessourcesModifier = 1.2f;
			if (Skills[20] && Skills[20]->Skill)
				RessourcesModifier = 1.3f;
		}
	}

	return RessourcesModifier;
}
float ASkillsTree::GetPlunderingModifier()
{
	float PlunderingModifier = 1.f;

	if (Skills[21] && Skills[21]->Skill)
	{
		PlunderingModifier = 1.2f;
		if (Skills[22] && Skills[22]->Skill)
		{
			PlunderingModifier = 1.4f;
			if (Skills[23] && Skills[23]->Skill)
				PlunderingModifier = 1.6f;
		}
	}

	return PlunderingModifier;
}
int ASkillsTree::GetConversionModifier()
{
	int ConversionModifier = 0;

	if (Skills[24] && Skills[24]->Skill)
	{
		ConversionModifier = 100;
		if (Skills[25] && Skills[25]->Skill)
		{
			ConversionModifier = 150;
			if (Skills[26] && Skills[26]->Skill)
				ConversionModifier = 200;
		}
	}

	return ConversionModifier;
}

float ASkillsTree::GetModeChangeModifier()
{
	float ModeChangeModifier = 1.f;

	if (Skills[27] && Skills[27]->Skill)
	{
		ModeChangeModifier = 1.2f;
		if (Skills[28] && Skills[28]->Skill)
		{
			ModeChangeModifier = 1.4f;
			if (Skills[29] && Skills[29]->Skill)
				ModeChangeModifier = 1.6f;
		}
	}

	return ModeChangeModifier;
}
float ASkillsTree::GetBuildingHealModifier()
{
	float BuildingHealModifier = 1.f;

	if (Skills[30] && Skills[30]->Skill)
	{
		BuildingHealModifier = 1.2f;
		if (Skills[31] && Skills[31]->Skill)
		{
			BuildingHealModifier = 1.4f;
			if (Skills[32] && Skills[32]->Skill)
				BuildingHealModifier = 1.6f;
		}
	}

	return BuildingHealModifier;
}
float ASkillsTree::GetSpawnUnityLifeModifier()
{
	float SpawnUnityLifeModifier = 0.2f;

	if (Skills[33] && Skills[33]->Skill)
	{
		SpawnUnityLifeModifier = 0.4f;
		if (Skills[34] && Skills[34]->Skill)
		{
			SpawnUnityLifeModifier = 0.6f;
			if (Skills[35] && Skills[35]->Skill)
				SpawnUnityLifeModifier = 0.8f;
		}
	}

	return SpawnUnityLifeModifier;
}

bool ASkillsTree::GetInvisibilityEnabled()
{
	return Skills[36] && Skills[36]->Skill;
}
float ASkillsTree::GetInvisibilityTimeModifier()
{
	if (Skills[37] && Skills[37]->Skill)
		return 1.4f;
	else
		return 1.f;
}
float ASkillsTree::GetInvisibilityCoolDownModifier()
{
	if (Skills[38] && Skills[38]->Skill)
		return 1.4f;
	else
		return 1.f;
}

void ASkillsTree::SetPoints(int Pts)
{
	if (Pts <= 20)
		Points = Pts;
	else
		Points = 20;
}

int ASkillsTree::SetSkills(TArray<ASkill*> SkillsChange)
{
	int PointsUsed = 0;

	for (int i(0); i < Skills.Num(); i++)
	{
		if (Skills[i]->Skill)
			PointsUsed++;
	}

	int PointsLeft = Points - PointsUsed;

	for (int i(0); i < SkillsChange.Num(); i++)
	{
		if (SkillsChange[i]->Skill && !Skills[i]->Skill && PointsLeft > 0)
		{
			if (!IsValid(SkillsChange[i]->Parent) || (SkillsChange[i]->Parent->Skill && Skills[i]->Parent->Skill))
			{
				Skills[i]->Skill = true;
				PointsLeft--;
			}
		}
	}

	return PointsLeft;
}
int ASkillsTree::SetSkills(TArray<bool> SkillsChange)
{
	int PointsUsed = 0;

	for (int i(0); i < Skills.Num(); i++)
	{
		if (Skills[i]->Skill)
			PointsUsed++;
	}

	int PointsLeft = Points - PointsUsed;

	for (int i(0); i < SkillsChange.Num(); i++)
	{
		if (SkillsChange[i] && !Skills[i]->Skill && PointsLeft > 0)
		{
			if (!IsValid(Skills[i]->Parent) || Skills[i]->Parent->Skill)
			{
				Skills[i]->Skill = true;
				PointsLeft--;
			}
		}
	}

	return PointsLeft;
}
TArray<ASkill*> ASkillsTree::GetSkills()
{
	return Skills;
}

//Replication
void ASkillsTree::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkillsTree, Points);
	DOREPLIFETIME(ASkillsTree, Skills);
}