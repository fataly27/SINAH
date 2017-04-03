// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Unit.h"


// Sets default values
AUnit::AUnit() : ImBlue(true), Selected(false), CurrentAction(Action::Idle), IsVisibleForOpponent(false)
{
	SetActorScale3D(FVector(0.7f));

	InvisibleLimitedTime = 15.f;
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
	DefaultReactionTime = 1.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualPhysicAttack = DefaultPhysicAttack;
	ActualMagicAttack = DefaultMagicAttack;
	ActualPhysicDefense = DefaultPhysicDefense;
	ActualMagicDefense = DefaultMagicDefense;
	ActualSpeed = DefaultSpeed;
	ActualFieldOfSight = DefaultFieldOfSight;
	ActualRange = DefaultRange;
	ActualReactionTime = DefaultReactionTime;

	CostInFood = 30;
	CostInCells = 5;
	CostInMetal = 5;
	CostInCristals = 0;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	GetMovementComponent()->SetJumpAllowed(false);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AUnitController::StaticClass();

	GetMesh()->bReceivesDecals = false;

	SelectionMark = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionMark"));
	SelectionMark->SetupAttachment(GetCapsuleComponent());

	SelectionMark->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UMaterial> RedCircleAsset(TEXT("/Game/Materials/RedCircle.RedCircle"));
	RedCircle = RedCircleAsset.Object;
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueCircleAsset(TEXT("/Game/Materials/BlueCircle.BlueCircle"));
	BlueCircle = BlueCircleAsset.Object;
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	Unselect();
	AmIBlue(ImBlue);
	WantedMode = Modes::Attack;

	if (ImBlue != (Role == ROLE_Authority))
		SetActorHiddenInGame(!IsVisibleForOpponent);
}

// Called every frame
void AUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (Role == ROLE_Authority)
	{
		InvisibleCoolDown -= DeltaTime;
		InvisibleLimitedTime -= DeltaTime;

		if (InvisibleLimitedTime <= 0.f && CurrentMode == Modes::Invisible)
		{
			WantedMode = Modes::Attack;
			Multicast_SetHidden(false, GetLocation(), GetActorRotation(), true);
		}
		if (WantedMode != Modes::None)
			PrepareChangingModeTime -= DeltaTime;
		SetMode();

		bool OpponentDied(false);
		TArray<TScriptInterface<IGameElementInterface>> NewSpecialTargets;
		for (int i = 0; i < SpecialTargets.Num(); i++)
		{
			if (SpecialTargets[i]->GetOpponentVisibility() && !SpecialTargets[i]->IsPendingKill())
				NewSpecialTargets.Add(SpecialTargets[i]);
			else if (SpecialTargets[i]->IsPendingKill())
				OpponentDied = true;
		}
		SpecialTargets = NewSpecialTargets;

		TArray<TScriptInterface<IGameElementInterface>> NewBoxSpecialTargets;
		for (int i = 0; i < BoxSpecialTargets.Num(); i++)
		{
			if (BoxSpecialTargets[i]->GetOpponentVisibility() && !BoxSpecialTargets[i]->IsPendingKill())
				NewBoxSpecialTargets.Add(BoxSpecialTargets[i]);
			else if (BoxSpecialTargets[i]->IsPendingKill())
				OpponentDied = true;
		}
		BoxSpecialTargets = NewBoxSpecialTargets;
		if(OpponentDied)
			Cast<AUnitController>(GetController())->BeginMove();
	}
	ChangeLoopingAnimation();

	if(SpecialTargets.IsValidIndex(0))
		FaceRotation(FRotationMatrix::MakeFromX(SpecialTargets[0]->GetLocation() - GetLocation()).Rotator());
	else if (Destinations.IsValidIndex(0))
		FaceRotation(FRotationMatrix::MakeFromX(Destinations[0] - GetLocation()).Rotator());
	else if (OpponentsInSight.IsValidIndex(0))
		FaceRotation(FRotationMatrix::MakeFromX(OpponentsInSight[0]->GetLocation() - GetLocation()).Rotator());
}

//Selection and color
void AUnit::Select()
{
	Selected = true;
	SelectionMark->SetHiddenInGame(false);
}
void AUnit::Unselect()
{
	Selected = false;
	SelectionMark->SetHiddenInGame(true);
}
bool AUnit::IsSelected()
{
	return Selected;
}
void AUnit::AmIBlue(bool color)
{
	ImBlue = color;

	if (ImBlue)
		SelectionMark->SetMaterial(0, BlueCircle);
	else
		SelectionMark->SetMaterial(0, RedCircle);
}
bool AUnit::TellIfImBlue()
{
	return ImBlue;
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
float AUnit::GetReactionTime()
{
	return ActualReactionTime;
}
float AUnit::GetHalfHeight()
{
	return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

//Destinations
void AUnit::AddDestination(FVector Destination, FRotator Rotation)
{
	if (CurrentMode != Modes::Defense)
	{
		FNavLocation CorrectDestination;
		FVector Extent = FVector(3500.f, 3500.f, 400.f);
		if (GetWorld()->GetNavigationSystem()->ProjectPointToNavigation(Destination, CorrectDestination, Extent, GetWorld()->GetNavigationSystem()->GetMainNavData()))
		{
			DesiredRotation = Rotation;
			Destinations.Add(CorrectDestination.Location);
			if (!Destinations.IsValidIndex(1) && !GetSpecialTargets().IsValidIndex(0))
			{
				Cast<AUnitController>(GetController())->BeginMove();
			}
		}
	}
}
void AUnit::ClearOneDestination()
{
	Destinations.RemoveAt(0);
}
void AUnit::ClearDestinations()
{
	Destinations.Empty();
	Multicast_SetIsMoving(Action::Idle);
}
TArray<FVector> AUnit::GetDestinations()
{
	return Destinations;
}
void AUnit::Rotate()
{
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
	for (int i(0); i < BoxSpecialTargets.Num(); i++)
		SpecialTargets.AddUnique(BoxSpecialTargets[i]);
	BoxSpecialTargets.Empty();
}
void AUnit::SetBoxSpecialTargets(TArray<TScriptInterface<IGameElementInterface>> NewTargets)
{
	// Removing doubles
	TArray<TScriptInterface<IGameElementInterface>> CleanedNewTargets;
	for (int i(0); i < NewTargets.Num(); i++)
	{
		if (!SpecialTargets.Contains(NewTargets[i]) && NewTargets[i]->TellIfImBlue() != TellIfImBlue() && NewTargets[i]->GetOpponentVisibility())
			CleanedNewTargets.Add(NewTargets[i]);
	}
	BoxSpecialTargets = CleanedNewTargets;
	if(CurrentMode != Modes::Attack && CurrentMode != Modes::Defense && WantedMode != Modes::Attack && WantedMode != Modes::Defense && NewTargets.Num() != 0)
		Server_ChangeMode(Modes::Attack);

	if(BoxSpecialTargets.IsValidIndex(0))
		Multicast_SetIsMoving(Action::Idle);
}
void AUnit::ClearSpecialTargets()
{
	SpecialTargets.Empty();
}
TArray<TScriptInterface<IGameElementInterface>> AUnit::GetSpecialTargets()
{
	TArray<TScriptInterface<IGameElementInterface>> ArrayToSend;
	ArrayToSend.Append(SpecialTargets);
	for (int i(0); i < BoxSpecialTargets.Num(); i++)
		ArrayToSend.AddUnique(BoxSpecialTargets[i]);

	return ArrayToSend;
}
void AUnit::SetOpponentsInSight(TArray<TScriptInterface<IGameElementInterface>> Opponents)
{
	if(CurrentMode == Modes::Attack || CurrentMode == Modes::Defense)
		OpponentsInSight = Opponents;
}
TArray<TScriptInterface<IGameElementInterface>> AUnit::GetOpponentsInSight()
{
	return OpponentsInSight;
}
void AUnit::ClearOpponentsInSight()
{
	OpponentsInSight.Empty();
}

//Attack
void AUnit::Server_Attack_Implementation(const TScriptInterface<IGameElementInterface>& Target)
{
	Target->ReceiveDamages(GetPhysicAttack(), GetMagicAttack());
}
bool AUnit::Server_Attack_Validate(const TScriptInterface<IGameElementInterface>& Target)
{
	return CurrentMode == Modes::Attack || CurrentMode == Modes::Defense;
}
void AUnit::Server_ReceiveDamages_Implementation(float Physic, float Magic)
{
	float PhysicDamage = Physic - GetPhysicDefense();
	float MagicDamage = Magic - GetMagicDefense();

	if (PhysicDamage <= 0 && Physic > 0)
		PhysicDamage = 1.f;
	if (MagicDamage <= 0 && Magic > 0)
		MagicDamage = 1.f;

	CurrentLife -= PhysicDamage;
	CurrentLife -= MagicDamage;

	if (CurrentLife <= 0.f)
		Destroy();
}
bool AUnit::Server_ReceiveDamages_Validate(float Physic, float Magic)
{
	return true;
}
void AUnit::ReceiveDamages(float Physic, float Magic)
{
	Server_ReceiveDamages(Physic, Magic);
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

			ActualSpeed = DefaultSpeed;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;

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

			ActualSpeed = 0.f;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;
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

			ActualSpeed = DefaultSpeed;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;
		}
		else if (Mode == Modes::Movement)
		{
			ClearOpponentsInSight();
			ClearSpecialTargets();

			ActualPhysicAttack = 0;
			ActualMagicAttack = 0;
			ActualPhysicDefense = DefaultPhysicDefense * 0.8f;
			ActualMagicDefense = DefaultMagicDefense * 0.8f;

			ActualFieldOfSight = DefaultFieldOfSight;

			ActualSpeed = DefaultSpeed * 1.6f;
			GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;

			CurrentMode = Mode;
		}
		else if (Mode == Modes::Invisible)
		{
			if (InvisibleCoolDown <= 0.f)
			{
				ClearOpponentsInSight();
				ClearSpecialTargets();

				InvisibleCoolDown = 180.f;
				InvisibleLimitedTime = 15.f;
				Multicast_SetHidden(true, GetLocation(), GetActorRotation(), true);

				ActualPhysicAttack = 0;
				ActualMagicAttack = 0;
				ActualPhysicDefense = 0;
				ActualMagicDefense = 0;

				ActualFieldOfSight = DefaultFieldOfSight;

				ActualSpeed = DefaultSpeed;
				GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;

				CurrentMode = Mode;
			}
		}
	}
}
void AUnit::Server_ChangeMode_Implementation(Modes Mode)
{
	if (Mode != Modes::Invisible || InvisibleCoolDown <= 0.f)
	{
		WantedMode = Mode;
		CurrentMode = Modes::None;

		ActualPhysicAttack = 0;
		ActualMagicAttack = 0;
		ActualPhysicDefense = 0;
		ActualMagicDefense = 0;

		ActualFieldOfSight = 0;

		ActualSpeed = 0.f;
		GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;

		PrepareChangingModeTime = 5.f;
	}
}
bool AUnit::Server_ChangeMode_Validate(Modes Mode)
{
	return true;
}
Modes AUnit::GetMode()
{
	return CurrentMode;
}
void AUnit::Multicast_SetIsMoving_Implementation(Action NewAction)
{
	CurrentAction = NewAction;

	if(NewAction != Action::Moving && Role == ROLE_Authority)
		Cast<AUnitController>(GetController())->StopMovement();
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
	{
		NewAnimation = AttackingAnimation;
	}
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
			GetMesh()->SetPlayRate(ActualSpeed);
		else
			GetMesh()->SetPlayRate(1.f);
	}
}

//Visibility
bool AUnit::GetOpponentVisibility()
{
	return IsVisibleForOpponent;
}
void AUnit::Multicast_SetHidden_Implementation(bool Hidden, FVector Position, FRotator Rotation, bool TurnIntoGhost)
{
	IsVisibleForOpponent = !Hidden;
	if (ImBlue == (Role == ROLE_Authority))
	{
		if (TurnIntoGhost)
		{
			if (Hidden)
				GetMesh()->SetSkeletalMesh(VisualAssetTransparent);
			else
				GetMesh()->SetSkeletalMesh(VisualAsset);

			GetMesh()->PlayAnimation(CurrentAnimation, true);
		}
	}
	else
	{
		SetActorLocation(Position);
		SetActorRotation(Rotation);
		SetActorHiddenInGame(Hidden);
	}
}
FVector AUnit::GetLocation()
{
	return GetActorLocation();
}


//Replication
void AUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(AUnit, SpecialTargets, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, BoxSpecialTargets, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, Destinations, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, ReplicatedMovement, COND_Custom);

	DOREPLIFETIME(AUnit, CurrentLife);
	DOREPLIFETIME(AUnit, ActualMaxLife);
	DOREPLIFETIME(AUnit, ActualPhysicAttack);
	DOREPLIFETIME(AUnit, ActualMagicAttack);
	DOREPLIFETIME(AUnit, ActualPhysicDefense);
	DOREPLIFETIME(AUnit, ActualMagicDefense);
	DOREPLIFETIME(AUnit, ActualSpeed);
	DOREPLIFETIME(AUnit, ActualFieldOfSight);
	DOREPLIFETIME(AUnit, ActualRange);

	DOREPLIFETIME(AUnit, CurrentMode);
	DOREPLIFETIME(AUnit, WantedMode);
	DOREPLIFETIME(AUnit, IsVisibleForOpponent);
}
void AUnit::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, SpecialTargets, !ImBlue);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, BoxSpecialTargets, !ImBlue);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, Destinations, !ImBlue);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, ReplicatedMovement, !ImBlue || IsVisibleForOpponent);
}