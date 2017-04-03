// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Unit.h"
#include "../GameElementInterface.h"
#include "UnitController.generated.h"

class AUnit;

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
		void BeginMove();
		void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override;

	protected:
		TScriptInterface<IGameElementInterface> LastTarget;
		float TimeSinceLastAction;
};
