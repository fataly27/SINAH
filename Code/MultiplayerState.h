// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "MultiplayerState.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMultiplayerState : public APlayerState
{
	GENERATED_BODY()

	public:
		AMultiplayerState();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

		int GetAmountOfFood();
		int GetAmountOfMetal();
		int GetAmountOfCells();
		int GetAmountOfCristals();

		void SetAmountOfFood(int Food);
		void SetAmountOfMetal(int Metal);
		void SetAmountOfCells(int Cells);
		void SetAmountOfCristals(int Cristals);

	protected:
		UPROPERTY(Replicated)
			int AmountOfFood;
		UPROPERTY(Replicated)
			int AmountOfMetal;
		UPROPERTY(Replicated)
			int AmountOfCells;
		UPROPERTY(Replicated)
			int AmountOfCristals;
};
