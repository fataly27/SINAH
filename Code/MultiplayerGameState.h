// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiplayerGameState.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMultiplayerGameState : public AGameState
{
	GENERATED_BODY()
	
	public:
		AMultiplayerGameState();

		void Tick(float DeltaTime) override;

		bool IsGameActive();
		void PreBeginGame();
		void CancelPreBeginGame();
		void BeginGame();

		FString GetStatusInfo();
		void SetStatusInfo(FString Text);

		int GetTime();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	private:
		UPROPERTY(Replicated)
			bool bGameActive;
		UPROPERTY(Replicated)
			ESide Winner;
		UPROPERTY(Replicated)
			FString StateInfo;
		UPROPERTY(Replicated)
			float CurrentTime;
		float CountDown;
};
