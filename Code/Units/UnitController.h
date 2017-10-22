// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "UnitController.generated.h"

class IGameElementInterface;

/**
 * 
 */
UCLASS()
class SINAH_API AUnitController : public AAIController
{
	GENERATED_BODY()

	public:
		AUnitController();

		void Tick( float DeltaSeconds ) override;
		void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override;

	protected:
		TScriptInterface<IGameElementInterface> LastTarget;
		FVector LastDestination;

		bool IsLastATarget;
};
