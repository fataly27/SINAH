// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "UnitController.h"

AUnitController::AUnitController() : Super(), TimeSinceLastAction(0.f) {}

void AUnitController::Tick(float DeltaSeconds)
{
	TimeSinceLastAction += DeltaSeconds;
	if (TimeSinceLastAction >= Cast<AUnit>(GetPawn())->GetReactionTime() && !Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0) && !Cast<AUnit>(GetPawn())->GetOpponentsInSight().IsValidIndex(0) && !Cast<AUnit>(GetPawn())->GetSpecialTargets().IsValidIndex(0))
	{
		TimeSinceLastAction = 0.f;
		Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Idle);
	}
	else if (TimeSinceLastAction >= Cast<AUnit>(GetPawn())->GetReactionTime() && (Cast<AUnit>(GetPawn())->GetSpecialTargets().IsValidIndex(0) || (Cast<AUnit>(GetPawn())->GetOpponentsInSight().IsValidIndex(0)) && !Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0)))
	{
		TimeSinceLastAction = 0.f;
		TArray<TScriptInterface<IGameElementInterface>> EffectiveTargets;
		if (Cast<AUnit>(GetPawn())->GetSpecialTargets().IsValidIndex(0))
			EffectiveTargets = Cast<AUnit>(GetPawn())->GetSpecialTargets();
		else if (Cast<AUnit>(GetPawn())->GetOpponentsInSight().IsValidIndex(0) && !Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0))
			EffectiveTargets = Cast<AUnit>(GetPawn())->GetOpponentsInSight();

		TScriptInterface<IGameElementInterface> ClosestTarget = EffectiveTargets[0];
		float Distance = FVector::Dist(GetPawn()->GetActorLocation(), EffectiveTargets[0]->GetLocation());
		for (int i = 1; i < EffectiveTargets.Num(); i++)
		{
			if (FVector::Dist(GetPawn()->GetActorLocation(), EffectiveTargets[i]->GetLocation()) < Distance)
			{
				Distance = FVector::Dist(GetPawn()->GetActorLocation(), EffectiveTargets[i]->GetLocation());
				ClosestTarget = EffectiveTargets[i];
			}
		}

		if (Distance <= Cast<AUnit>(GetPawn())->GetRange() * 100)
		{
			Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Attacking);
			Cast<AUnit>(GetPawn())->Attack(ClosestTarget);
		}
		else if (ClosestTarget != LastTarget || GetMoveStatus() != EPathFollowingStatus::Moving)
		{
			StopMovement();
			MoveToActor(Cast<AActor>(ClosestTarget.GetObject()), Cast<AUnit>(GetPawn())->GetRange() * 100, false, true, false);
			Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Moving);
		}
		LastTarget = ClosestTarget;
	}
}

void AUnitController::BeginMove()
{
	if (!Cast<AUnit>(GetPawn())->GetSpecialTargets().IsValidIndex(0))
	{
		if (Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0))
		{
			MoveToLocation(Cast<AUnit>(GetPawn())->GetDestinations()[0], 10.f, false, true, true, false);
			Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Moving);
		}
		else
			Cast<AUnit>(GetPawn())->Rotate();
	}
}

void AUnitController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	if (Result.IsSuccess())
	{
		if (!Cast<AUnit>(GetPawn())->GetSpecialTargets().IsValidIndex(0))
		{
			if (Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0))
				Cast<AUnit>(GetPawn())->ClearOneDestination();
			if (!Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0))
				Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Idle);
			BeginMove();
		}
	}
}