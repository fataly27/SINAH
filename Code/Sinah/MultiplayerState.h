// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Widgets/GameBeforeStartingWidget.h"
#include "MultiplayerState.generated.h"

class ASkillsTree;

/**
 * 
 */
UCLASS()
class SINAH_API AMultiplayerState : public APlayerState
{
	GENERATED_BODY()

	public:
		AMultiplayerState();

		virtual void BeginPlay() override;

		int GetAmountOfFood();
		int GetAmountOfMetal();
		int GetAmountOfCells();
		int GetAmountOfCristals();

		void SetAmountOfFood(int Food);
		void SetAmountOfMetal(int Metal);
		void SetAmountOfCells(int Cells);
		void SetAmountOfCristals(int Cristals);

		int GetFoodChange();
		int GetMetalChange();
		int GetCellsChange();
		int GetCristalsChange();

		void SetFoodChange(int Food);
		void SetMetalChange(int Metal);
		void SetCellsChange(int Cells);
		void SetCristalsChange(int Cristals);

		void SetCivChosen(ECivs Civ);

		ASkillsTree* GetSkillsTree();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	protected:
		//Ressources
		UPROPERTY(Replicated)
			int AmountOfFood;
		UPROPERTY(Replicated)
			int AmountOfMetal;
		UPROPERTY(Replicated)
			int AmountOfCells;
		UPROPERTY(Replicated)
			int AmountOfCristals;

		UPROPERTY(Replicated)
			int FoodChange;
		UPROPERTY(Replicated)
			int MetalChange;
		UPROPERTY(Replicated)
			int CellsChange;
		UPROPERTY(Replicated)
			int CristalsChange;

		//Civilisation
		UPROPERTY(Replicated)
			ECivs CivChosen;

		//Skills Tree
		UPROPERTY(Replicated)
			ASkillsTree* SkillsTree;
};
