// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "UnitController.h"
#include "../MousePlayerController.h"
#include "Unit.h"


// Sets default values
AUnit::AUnit() : MySide(Side::Neutral), Selected(false), CurrentAction(Action::Idle), IsVisibleForOpponent(false)
{
	bReplicates = true;
	bReplicateMovement = true;

	InvisibleLimitedTime = 0.f;
	InvisibleCoolDown = 0.f;
	PrepareChangingModeTime = 0.f;

	DefaultMaxLife = 100;
	DefaultPhysicAttack = 10;
	DefaultMagicAttack = 0;
	DefaultPhysicDefense = 5;
	DefaultMagicDefense = 0;
	DefaultSpeed = 3.5f;
	DefaultFieldOfSight = 10.f;
	DefaultRange = 5.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualPhysicAttack = DefaultPhysicAttack;
	ActualMagicAttack = DefaultMagicAttack;
	ActualPhysicDefense = DefaultPhysicDefense;
	ActualMagicDefense = DefaultMagicDefense;
	ActualSpeed = DefaultSpeed;
	SpeedMultiplicator = 1.f;
	ActualFieldOfSight = DefaultFieldOfSight;
	ActualRange = DefaultRange;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	GetMovementComponent()->SetJumpAllowed(false);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AUnitController::StaticClass();

	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRenderCustomDepth(false);
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	Unselect();
	WantedMode = Modes::Attack;
	SetSide(MySide);

	SpawnDefaultController();

	if ((MySide == Side::Blue && Role != ROLE_Authority) || (MySide == Side::Red && Role == ROLE_Authority))
		SetActorHiddenInGame(!IsVisibleForOpponent);
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		InvisibleCoolDown -= DeltaTime;
		InvisibleLimitedTime -= DeltaTime;

		if (InvisibleLimitedTime <= 0.f && CurrentMode == Modes::Invisible)
		{
			ChangeMode(Modes::Attack);
			InvisibleCoolDown = 180.f;

			Multicast_SetInvisibleAsset(GetMode() == Modes::Invisible);
		}
		if (WantedMode != Modes::None)
			PrepareChangingModeTime -= DeltaTime;
		SetMode();

		TArray<TScriptInterface<IGameElementInterface>> NewSpecialTargets;
		for (int i = 0; i < SpecialTargets.Num(); i++)
		{
			if (SpecialTargets[i]->GetOpponentVisibility() && !SpecialTargets[i]->IsPendingKill() && SpecialTargets[i]->GetSide() != GetSide())
				NewSpecialTargets.Add(SpecialTargets[i]);
		}
		SpecialTargets = NewSpecialTargets;

		TArray<TScriptInterface<IGameElementInterface>> NewBoxSpecialTargets;
		for (int i = 0; i < BoxSpecialTargets.Num(); i++)
		{
			if (BoxSpecialTargets[i]->GetOpponentVisibility() && !BoxSpecialTargets[i]->IsPendingKill() && BoxSpecialTargets[i]->GetSide() != GetSide())
				NewBoxSpecialTargets.Add(BoxSpecialTargets[i]);
		}
		BoxSpecialTargets = NewBoxSpecialTargets;

		if (SpecialTargets.IsValidIndex(0))
		{
			FVector NearestTarget = SpecialTargets[0]->GetLocation();

			for (int i(1); i < SpecialTargets.Num(); i++)
			{
				if (FVector::Dist(GetLocation(), SpecialTargets[i]->GetLocation()) < FVector::Dist(GetLocation(), NearestTarget))
					NearestTarget = SpecialTargets[i]->GetLocation();
			}

			FaceRotation(FRotationMatrix::MakeFromX(NearestTarget - GetLocation()).Rotator());
		}
		else if (Destinations.IsValidIndex(0))
			FaceRotation(FRotationMatrix::MakeFromX(Destinations[0] - GetLocation()).Rotator());
		else if (OpponentsInSight.IsValidIndex(0))
			FaceRotation(FRotationMatrix::MakeFromX(OpponentsInSight[0]->GetLocation() - GetLocation()).Rotator());

		SpecialTargetsActors.Empty();
		for (int i(0); i < SpecialTargets.Num(); i++)
		{
			SpecialTargetsActors.Add(Cast<AActor>(SpecialTargets[i].GetObject()));
		}

		BoxSpecialTargetsActors.Empty();
		for (int i(0); i < BoxSpecialTargets.Num(); i++)
		{
			BoxSpecialTargetsActors.Add(Cast<AActor>(BoxSpecialTargets[i].GetObject()));
		}
	}
	ChangeLoopingAnimation();
}

//Unit
FName AUnit::GetName()
{
	return Name;
}
UTexture* AUnit::GetUnitImage()
{
	return UnitImage;
}

//Selection and side
void AUnit::Select()
{
	Selected = true;
	GetMesh()->SetRenderCustomDepth(true);
}
void AUnit::Unselect()
{
	Selected = false;
	GetMesh()->SetRenderCustomDepth(false);
}
bool AUnit::IsSelected()
{
	return Selected;
}
void AUnit::SetSide(Side NewSide)
{
	Multicast_SetSide(NewSide);
}
void AUnit::Multicast_SetSide_Implementation(Side NewSide)
{
	Unselect();
	MySide = NewSide;

	int Color(0);

	if (MySide == Side::Blue)
	{
		Color = STENCIL_BLUE_OUTLINE;
	}
	else if (MySide == Side::Red)
	{
		Color = STENCIL_RED_OUTLINE;
	}
	else
		Color = STENCIL_GREY_OUTLINE

		GetMesh()->SetCustomDepthStencilValue(Color);
}
Side AUnit::GetSide()
{
	return MySide;
}

//Statistics
int AUnit::GetMaxLife()
{
	return ActualMaxLife;
}
int AUnit::GetCurrentLife()
{
	return CurrentLife;
}
int AUnit::GetPhysicAttack()
{
	return ActualPhysicAttack;
}
int AUnit::GetMagicAttack()
{
	return ActualMagicAttack;
}
int AUnit::GetPhysicDefense()
{
	return ActualPhysicDefense;
}
int AUnit::GetMagicDefense()
{
	return ActualMagicDefense;
}
float AUnit::GetSpeed()
{
	return ActualSpeed;
}
float AUnit::GetFieldOfSight()
{
	return ActualFieldOfSight;
}
float AUnit::GetRange()
{
	return ActualRange;
}
float AUnit::GetHalfHeight()
{
	return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}
unsigned int AUnit::GetLifeBarWidth()
{
	return 0;
}
float AUnit::GetSize()
{
	return 0.f;
}
unsigned int AUnit::GetBuildingLevelRequired()
{
	return BuildingLevelRequired;
}

int AUnit::GetFoodEatenInHalfASecond()
{
	return FoodEatenInHalfASecond;
}
int AUnit::GetCostInFood()
{
	return COST_IN_FOOD;
}
int AUnit::GetCostInCells()
{
	return COST_IN_CELLS;
}
int AUnit::GetCostInMetal()
{
	return COST_IN_METAL;
}
int AUnit::GetCostInCristals()
{
	return COST_IN_CRISTALS;
}

void AUnit::SetSpeedMultiplicator(float Multiplicator)
{
	SpeedMultiplicator = Multiplicator;

	GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;
}

//Destinations
void AUnit::AddDestination(FVector Destination, FRotator Rotation)
{
	if (CurrentMode != Modes::Defense && Role == ROLE_Authority)
	{
		FNavLocation CorrectDestination;
		FVector Extent = FVector(3500.f, 3500.f, 400.f);
		if (GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(Destination, CorrectDestination, Extent, GetWorld()->GetNavigationSystem()->GetMainNavData()))
		{
			DesiredRotation = Rotation;
			Destinations.Add(CorrectDestination.Location);
		}
	}
}
void AUnit::ClearOneDestination()
{
	if (Role == ROLE_Authority)
		Destinations.RemoveAt(0);
}
void AUnit::ClearDestinations()
{
	if (Role == ROLE_Authority)
	{
		Destinations.Empty();
	}
}
TArray<FVector> AUnit::GetDestinations()
{
	return Destinations;
}
void AUnit::Rotate()
{
	if (Role == ROLE_Authority)
		FaceRotation(DesiredRotation);
}
FVector AUnit::GetLocationAfterAllMoves()
{
	if (Destinations.IsValidIndex(0))
		return Destinations.Top();
	else
		return GetLocation();
}

//Target
void AUnit::AddSpecialTargets()
{
	if (Role == ROLE_Authority)
	{
		for (int i(0); i < BoxSpecialTargets.Num(); i++)
			SpecialTargets.AddUnique(BoxSpecialTargets[i]);
		BoxSpecialTargets.Empty();
	}
}
void AUnit::SetBoxSpecialTargets(TArray<TScriptInterface<IGameElementInterface>> NewTargets)
{
	if (Role == ROLE_Authority)
	{
		// Removing doubles
		TArray<TScriptInterface<IGameElementInterface>> CleanedNewTargets;
		for (int i(0); i < NewTargets.Num(); i++)
		{
			if (!SpecialTargets.Contains(NewTargets[i]) && NewTargets[i]->GetSide() != GetSide() && NewTargets[i]->GetOpponentVisibility())
				CleanedNewTargets.Add(NewTargets[i]);
		}
		BoxSpecialTargets = CleanedNewTargets;
		if (CurrentMode != Modes::Attack && CurrentMode != Modes::Defense && WantedMode != Modes::Attack && WantedMode != Modes::Defense && NewTargets.Num() != 0)
			ChangeMode(Modes::Attack);
	}
}
void AUnit::ClearSpecialTargets()
{
	if (Role == ROLE_Authority)
		SpecialTargets.Empty();
}
TArray<TScriptInterface<IGameElementInterface>> AUnit::GetSpecialTargets()
{
	TArray<AActor*> ArrayBeforeSending;
	TArray<TScriptInterface<IGameElementInterface>> ArrayToSend;

	ArrayBeforeSending.Append(SpecialTargetsActors);
	for (int i(0); i < BoxSpecialTargetsActors.Num(); i++)
		ArrayBeforeSending.AddUnique(BoxSpecialTargetsActors[i]);

	for (int i(0); i < ArrayBeforeSending.Num(); i++)
	{
		TScriptInterface<IGameElementInterface> NewInterface;
		NewInterface.SetInterface(Cast<IGameElementInterface>(ArrayBeforeSending[i]));
		NewInterface.SetObject(ArrayBeforeSending[i]);

		ArrayToSend.Add(NewInterface);
	}

	return ArrayToSend;
}
void AUnit::SetOpponentsInSight(TArray<TScriptInterface<IGameElementInterface>> Opponents)
{
	if (Role == ROLE_Authority && (CurrentMode == Modes::Attack || CurrentMode == Modes::Defense))
		OpponentsInSight = Opponents;
}
TArray<TScriptInterface<IGameElementInterface>> AUnit::GetOpponentsInSight()
{
	return OpponentsInSight;
}
void AUnit::ClearOpponentsInSight()
{
	if (Role == ROLE_Authority)
		OpponentsInSight.Empty();
}

//Attack
void AUnit::Attack(const TScriptInterface<IGameElementInterface>& Target)
{
	if (Role == ROLE_Authority && (CurrentMode == Modes::Attack || CurrentMode == Modes::Defense) && Target->GetOpponentVisibility())
		Target->ReceiveDamages(GetPhysicAttack(), GetMagicAttack(), MySide);
}
void AUnit::ReceiveDamages(int Physic, int Magic, Side AttackingSide)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		int PhysicDamage = Physic - GetPhysicDefense();
		int MagicDamage = Magic - GetMagicDefense();

		if (PhysicDamage <= 0 && Physic > 0)
			PhysicDamage = 1.f;
		if (MagicDamage <= 0 && Magic > 0)
			MagicDamage = 1.f;

		int Damages = PhysicDamage + MagicDamage;

		if (CurrentLife <= Damages)
			Destroy();
		else
			CurrentLife -= Damages;
	}
}
void AUnit::Heal(int Heal)
{
	if (Role == ROLE_Authority)
	{
		if (CurrentLife + Heal <= 0)
			Destroy();
		else
		{
			CurrentLife += Heal;
			if (CurrentLife > GetMaxLife())
				CurrentLife = GetMaxLife();
		}
	}
}

//Dying
bool AUnit::IsPendingKill()
{
	return IsPendingKillPending();
}

//Modes and animation
void AUnit::SetMode()
{
	if (Role == ROLE_Authority && PrepareChangingModeTime <= 0.f && WantedMode != Modes::None)
	{
		Modes Mode = WantedMode;
		WantedMode = Modes::None;
		if (Mode == Modes::Attack)
		{
			ActualPhysicAttack = DefaultPhysicAttack * 1.2f;
			ActualMagicAttack = DefaultMagicAttack * 1.2f;
			ActualPhysicDefense = DefaultPhysicDefense * 0.8f;
			ActualMagicDefense = DefaultMagicDefense * 0.8f;

			ActualFieldOfSight = DefaultFieldOfSight;
			ActualRange = DefaultRange;

			ActualSpeed = DefaultSpeed;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;

			CurrentMode = Mode;
		}
		else if (Mode == Modes::Defense)
		{
			CurrentMode = Mode;

			ActualPhysicAttack = DefaultPhysicAttack;
			ActualMagicAttack = DefaultMagicAttack;
			ActualPhysicDefense = DefaultPhysicDefense * 1.6f;
			ActualMagicDefense = DefaultMagicDefense * 1.6f;

			ActualFieldOfSight = DefaultFieldOfSight;
			ActualRange = DefaultRange;

			ActualSpeed = 0.f;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;

			ClearDestinations();
		}
		else if (Mode == Modes::Alert)
		{
			ClearOpponentsInSight();
			ClearSpecialTargets();

			CurrentMode = Mode;

			ActualPhysicAttack = 0;
			ActualMagicAttack = 0;
			ActualPhysicDefense = DefaultPhysicDefense;
			ActualMagicDefense = DefaultMagicDefense;

			ActualFieldOfSight = DefaultFieldOfSight * 1.4f;
			ActualRange = 0;

			ActualSpeed = DefaultSpeed;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;
		}
		else if (Mode == Modes::Movement)
		{
			ClearOpponentsInSight();
			ClearSpecialTargets();

			ActualPhysicAttack = 0;
			ActualMagicAttack = 0;
			ActualPhysicDefense = DefaultPhysicDefense * 0.8f;
			ActualMagicDefense = DefaultMagicDefense * 0.8f;

			ActualRange = 0;

			ActualFieldOfSight = DefaultFieldOfSight;

			ActualSpeed = DefaultSpeed * 1.6f;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;

			CurrentMode = Mode;
		}
		else if (Mode == Modes::Invisible)
		{
			if (InvisibleCoolDown <= 0.f)
			{
				ClearOpponentsInSight();
				ClearSpecialTargets();

				InvisibleLimitedTime = 15.f;

				ActualPhysicAttack = 0;
				ActualMagicAttack = 0;
				ActualPhysicDefense = 0;
				ActualMagicDefense = 0;

				ActualRange = 0;

				ActualFieldOfSight = DefaultFieldOfSight;

				ActualSpeed = DefaultSpeed;
				GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;

				CurrentMode = Mode;

				Multicast_SetInvisibleAsset(GetMode() == Modes::Invisible);
			}
		}
	}
}
void AUnit::ChangeMode(Modes Mode)
{
	if ((Mode != Modes::Invisible || InvisibleCoolDown <= 0.f) && CurrentMode != Mode && Role == ROLE_Authority)
	{
		if (CurrentMode == Modes::Invisible)
		{
			InvisibleCoolDown = 180.f - InvisibleLimitedTime / 15.f * 180.f;
			InvisibleLimitedTime = 0.f;

			CurrentMode = Modes::None;

			Multicast_SetInvisibleAsset(GetMode() == Modes::Invisible);
		}

		WantedMode = Mode;
		CurrentMode = Modes::None;

		ActualPhysicAttack = 0;
		ActualMagicAttack = 0;
		ActualPhysicDefense = 0;
		ActualMagicDefense = 0;

		ActualFieldOfSight = 0;
		ActualRange = 0;

		ActualSpeed = 0.f;
		GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * SpeedMultiplicator * 100;

		PrepareChangingModeTime = 2.f;
	}
}
Modes AUnit::GetMode()
{
	return CurrentMode;
}
void AUnit::Multicast_SetIsMoving_Implementation(Action NewAction)
{
	CurrentAction = NewAction;
}
void AUnit::ChangeLoopingAnimation()
{
	UAnimationAsset* NewAnimation;
	if (CurrentAction == Action::Moving)
	{
		if (CurrentMode == Modes::Movement)
			NewAnimation = RunningAnimation;
		else
			NewAnimation = WalkingAnimation;
	}
	else if (CurrentAction == Action::Attacking)
		NewAnimation = AttackingAnimation;
	else
	{
		if (CurrentMode == Modes::Defense)
			NewAnimation = ReadyIdleAnimation;
		else
			NewAnimation = NeutralIdleAnimation;
	}
	if (NewAnimation != CurrentAnimation)
	{
		CurrentAnimation = NewAnimation;
		GetMesh()->PlayAnimation(CurrentAnimation, true);
		if (CurrentAnimation == WalkingAnimation || CurrentAnimation == RunningAnimation)
			GetMesh()->SetPlayRate(ActualSpeed * SpeedMultiplicator);
		else
			GetMesh()->SetPlayRate(1.f);
	}
}
float AUnit::GetInvisibleCoolDown()
{
	return InvisibleCoolDown;
}
float AUnit::GetInvisibleTime()
{
	return InvisibleLimitedTime;
}

//Visibility
bool AUnit::GetOpponentVisibility()
{
	return IsVisibleForOpponent;
}
void AUnit::Multicast_SetVisibility_Implementation(bool Visibility, FVector Position, FRotator Rotation)
{
	IsVisibleForOpponent = Visibility;
	if (Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->GetSide() != GetSide())
	{
		SetActorLocation(Position);
		SetActorRotation(Rotation);
		SetActorHiddenInGame(!Visibility);
	}
}
void AUnit::Multicast_SetInvisibleAsset_Implementation(bool IsInvisible)
{
	if (IsInvisible)
		GetMesh()->SetSkeletalMesh(VisualAssetTransparent);
	else
		GetMesh()->SetSkeletalMesh(VisualAsset);

	GetMesh()->PlayAnimation(CurrentAnimation, true);
}
FVector AUnit::GetLocation()
{
	return GetActorLocation();
}


//Replication
void AUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(AUnit, SpecialTargetsActors, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, BoxSpecialTargetsActors, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, Destinations, COND_Custom);

	DOREPLIFETIME_CONDITION(AUnit, ReplicatedMovement, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, ReplicatedMovement, COND_Custom);

	DOREPLIFETIME(AUnit, CurrentLife);
	DOREPLIFETIME(AUnit, ActualMaxLife);
	DOREPLIFETIME(AUnit, ActualPhysicAttack);
	DOREPLIFETIME(AUnit, ActualMagicAttack);
	DOREPLIFETIME(AUnit, ActualPhysicDefense);
	DOREPLIFETIME(AUnit, ActualMagicDefense);
	DOREPLIFETIME(AUnit, ActualSpeed);
	DOREPLIFETIME(AUnit, SpeedMultiplicator);
	DOREPLIFETIME(AUnit, ActualFieldOfSight);
	DOREPLIFETIME(AUnit, ActualRange);

	DOREPLIFETIME(AUnit, InvisibleLimitedTime);
	DOREPLIFETIME(AUnit, InvisibleCoolDown);

	DOREPLIFETIME(AUnit, CurrentMode);
	DOREPLIFETIME(AUnit, WantedMode);
	DOREPLIFETIME(AUnit, IsVisibleForOpponent);
}
void AUnit::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, SpecialTargetsActors, MySide == Side::Red);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, BoxSpecialTargetsActors, MySide == Side::Red);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, Destinations, MySide == Side::Red);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, ReplicatedMovement, MySide == Side::Red || IsVisibleForOpponent);
}