// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Unit.h"
#include "UnitController.h"

AUnitController::AUnitController() : Super(), IsLastATarget(false) {}

void AUnitController::Tick(float DeltaSeconds)
{
	Modes UnitMode(Cast<AUnit>(GetPawn())->GetMode());

	if ((Cast<AUnit>(GetPawn())->GetSpecialTargets().IsValidIndex(0) || (Cast<AUnit>(GetPawn())->GetOpponentsInSight().IsValidIndex(0) && !Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0))) && (UnitMode == Modes::Attack || UnitMode == Modes::Defense))
	{
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

		if (Distance <= Cast<AUnit>(GetPawn())->GetRange() * 100 + ClosestTarget->GetSize())
		{
			StopMovement();
			Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Attacking);
			Cast<AUnit>(GetPawn())->Attack(ClosestTarget);
		}
		else if (ClosestTarget != LastTarget || GetMoveStatus() != EPathFollowingStatus::Moving || !IsLastATarget)
		{
			IsLastATarget = true;

			if (UnitMode == Modes::Attack)
			{
				StopMovement();

				AActor* Target = Cast<AActor>(ClosestTarget.GetObject());
				TScriptInterface<IGameElementInterface> TargetInterface;
				TargetInterface.SetInterface(Cast<IGameElementInterface>(Target));
				TargetInterface.SetObject(Target);
				FVector TargetPosition = Target->GetActorLocation();

				FVector PawnPosition = Cast<AUnit>(GetPawn())->GetActorLocation();

				float Scale = TargetInterface->GetSize() / FVector::Dist2D(TargetPosition, PawnPosition);
				float XOffset = Scale * (PawnPosition.X - TargetPosition.X);
				float YOffset = Scale * (PawnPosition.Y - TargetPosition.Y);

				FVector NewDestination(TargetPosition.X + XOffset, TargetPosition.Y + YOffset, TargetPosition.Z);

				FNavLocation ReachableDestination;

				if (GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(NewDestination, ReachableDestination, FVector(Cast<AUnit>(GetPawn())->GetRange() * 100 * FGenericPlatformMath::Sqrt(2), Cast<AUnit>(GetPawn())->GetRange() * 100 * FGenericPlatformMath::Sqrt(2), 500.f)))
				{
					MoveToLocation(ReachableDestination.Location, 20.f, false, true, false);
					Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Moving);
				}
			}
			else
			{
				StopMovement();
				Cast<AUnit>(GetPawn())->Rotate();
				Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Idle);
			}
		}
		LastTarget = ClosestTarget;
	}
	else if (Cast<AUnit>(GetPawn())->GetDestinations().IsValidIndex(0) && UnitMode != Modes::Defense && UnitMode != Modes::None)
	{
		if (Cast<AUnit>(GetPawn())->GetDestinations()[0] != LastDestination || GetMoveStatus() != EPathFollowingStatus::Moving || IsLastATarget)
		{
			IsLastATarget = false;
			MoveToLocation(Cast<AUnit>(GetPawn())->GetDestinations()[0], 10.f, false, true, true, false);
			Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Moving);
		}
	}
	else
	{
		StopMovement();
		Cast<AUnit>(GetPawn())->Rotate();
		Cast<AUnit>(GetPawn())->Multicast_SetIsMoving(Action::Idle);
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
		}
	}
}