// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "UnitController.h"
#include "../MousePlayerController.h"
#include "../MultiplayerState.h"
#include "../SkillsTree.h"
#include "Unit.h"


// Sets default values
AUnit::AUnit() : MySide(ESide::Neutral), bSelected(false), CurrentAction(EAction::Idle), bVisibleForOpponent(false), Civ(ECivs::None)
{
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 2;

	GetCharacterMovement()->bUseRVOAvoidance = false;
	GetCharacterMovement()->bCanWalkOffLedges = false;
	GetCharacterMovement()->NetworkSmoothingMode = ENetworkSmoothingMode::Linear;
	GetCharacterMovement()->SetJumpAllowed(false);

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

	AdaptSpeed = 1.f;
	AdaptScale = 1.f;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AUnitController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetRenderCustomDepth(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> BlueSparkAsset(TEXT("/Game/Particles/Sparks_Blue.Sparks_Blue"));
	BlueSpark = BlueSparkAsset.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RedSparkAsset(TEXT("/Game/Particles/Sparks_Red.Sparks_Red"));
	RedSpark = RedSparkAsset.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionAsset(TEXT("/Game/Particles/Explosion.Explosion"));
	Explosion = ExplosionAsset.Object;

	SetActorScale3D(FVector(1.5f));
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	Unselect();
	WantedMode = EModes::Attack;

	SpawnDefaultController();

	if (Role == ROLE_Authority)
		Multicast_SetVisibility(false, GetLocation(), GetActorRotation());
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->GetSide() == GetSide() && bHidden == true)
		SetActorHiddenInGame(false);

	if (Role == ROLE_Authority)
	{
		if (Player->GetSkillsTree()->GetInvisibilityEnabled())
		{
			if (Player && Player->GetSkillsTree())
			{
				InvisibleCoolDown -= DeltaTime * Player->GetSkillsTree()->GetInvisibilityCoolDownModifier();
				InvisibleLimitedTime -= DeltaTime / Player->GetSkillsTree()->GetInvisibilityTimeModifier();
			}
			else
			{
				InvisibleCoolDown -= DeltaTime;
				InvisibleLimitedTime -= DeltaTime;
			}
		}
		else
			InvisibleCoolDown = 0.f;

		if (InvisibleLimitedTime <= 0.f && CurrentMode == EModes::Invisible)
		{
			ChangeMode(EModes::Attack);
			InvisibleCoolDown = 180.f;

			Multicast_SetInvisibleAsset(GetMode() == EModes::Invisible);
		}
		if (WantedMode != EModes::None)
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
	bSelected = true;
	GetMesh()->SetRenderCustomDepth(true);
}
void AUnit::Unselect()
{
	bSelected = false;
	GetMesh()->SetRenderCustomDepth(false);
}
bool AUnit::IsSelected()
{
	return bSelected;
}
void AUnit::SetSide(ESide NewSide, AMultiplayerState* NewPlayer)
{
	Multicast_SetSide(NewSide, NewPlayer);
}
void AUnit::Multicast_SetSide_Implementation(ESide NewSide, AMultiplayerState* NewPlayer)
{
	Unselect();
	MySide = NewSide;
	Player = NewPlayer;

	if (Player && Player->GetSkillsTree())
		CurrentLife = ActualMaxLife * Player->GetSkillsTree()->GetSpawnUnityLifeModifier();

	int Color(0);

	if (MySide == ESide::Blue)
	{
		Color = STENCIL_BLUE_OUTLINE;
	}
	else if (MySide == ESide::Red)
	{
		Color = STENCIL_RED_OUTLINE;
	}
	else
		Color = STENCIL_GREY_OUTLINE

	GetMesh()->SetCustomDepthStencilValue(Color);
}
ESide AUnit::GetSide()
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
	if (Player && Player->GetSkillsTree())
		return ActualPhysicAttack * Player->GetSkillsTree()->GetPhysicAttackModifier();
	else
		return ActualPhysicAttack;
}
int AUnit::GetMagicAttack()
{
	if (Player && Player->GetSkillsTree())
		return ActualMagicAttack * Player->GetSkillsTree()->GetMagicAttackModifier();
	else
		return ActualMagicAttack;
}
int AUnit::GetPhysicDefense()
{
	if (Player && Player->GetSkillsTree())
		return ActualPhysicDefense * Player->GetSkillsTree()->GetPhysicDefenseModifier();
	else
		return ActualPhysicDefense;
}
int AUnit::GetMagicDefense()
{
	if (Player && Player->GetSkillsTree())
		return ActualMagicDefense * Player->GetSkillsTree()->GetMagicDefenseModifier();
	else
		return ActualMagicDefense;
}
float AUnit::GetSpeed()
{
	return ActualSpeed * SpeedMultiplicator;
}
float AUnit::GetFieldOfSight()
{
	if (Player && Player->GetSkillsTree())
		return ActualFieldOfSight * Player->GetSkillsTree()->GetUnitSightModifier();
	else
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
float AUnit::GetSize()
{
	return GetCapsuleComponent()->GetScaledCapsuleRadius();
}
unsigned int AUnit::GetLifeBarWidth()
{
	return 500;
}
unsigned int AUnit::GetBuildingLevelRequired()
{
	return BuildingLevelRequired;
}

int AUnit::GetFoodEatenInASecond()
{
	return FoodEatenInASecond;
}
int AUnit::GetCostInFood()
{
	return CostInFood;
}
int AUnit::GetCostInCells()
{
	return CostInCells;
}
int AUnit::GetCostInMetal()
{
	return CostInMetal;
}
int AUnit::GetCostInCristals()
{
	return CostInCristal;
}

void AUnit::SetSpeedMultiplicator(float Multiplicator)
{
	if (SpeedMultiplicator != Multiplicator)
	{
		SpeedMultiplicator = Multiplicator;

		GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;

		ChangeLoopingAnimation(true);
	}
}

//Destinations
void AUnit::AddDestination(FVector Destination, FRotator Rotation)
{
	if (CurrentMode != EModes::Defense && Role == ROLE_Authority)
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
		if (CurrentMode != EModes::Attack && CurrentMode != EModes::Defense && WantedMode != EModes::Attack && WantedMode != EModes::Defense && CleanedNewTargets.Num() != 0)
			ChangeMode(EModes::Attack);
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
	if (Role == ROLE_Authority && (CurrentMode == EModes::Attack || CurrentMode == EModes::Defense))
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
	if (Role == ROLE_Authority && (CurrentMode == EModes::Attack || CurrentMode == EModes::Defense) && Target->GetOpponentVisibility())
	{
		float Distance = FVector::Dist(this->GetActorLocation(), Target->GetLocation());

		if (Distance <= this->GetRange() * 100 + Target->GetSize() + GetSize())
		{
			if (MySide == ESide::Blue)
				Multicast_Spark(BlueSpark);
			else if (MySide == ESide::Red)
				Multicast_Spark(RedSpark);

			Target->ReceiveDamages(GetPhysicAttack(), GetMagicAttack(), MySide, Player);
		}
	}
}
void AUnit::Multicast_Spark_Implementation(UParticleSystem* Particle)
{
	UParticleSystemComponent* NewParticle = UGameplayStatics::SpawnEmitterAttached(Particle, GetMesh(), NAME_None, FVector(0.f, 60.f / AdaptScale, 120.f / AdaptScale), FRotator(0.f, 0.f, 90.f), EAttachLocation::SnapToTarget, true);
}
void AUnit::Multicast_Explosion_Implementation()
{
	UParticleSystemComponent* NewParticle = UGameplayStatics::SpawnEmitterAttached(Explosion, GetMesh(), NAME_None, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), EAttachLocation::SnapToTarget, true);
}
void AUnit::ReceiveDamages(int Physic, int Magic, ESide AttackingSide, AMultiplayerState* AttackingPlayer)
{
	if (Role == ROLE_Authority && MySide != AttackingSide)
	{
		if (FMath::RandRange(1, 8) == 1)
			Multicast_Explosion();

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

//EModes and animation
void AUnit::SetMode()
{
	if (Role == ROLE_Authority && PrepareChangingModeTime <= 0.f && WantedMode != EModes::None)
	{
		EModes Mode = WantedMode;
		WantedMode = EModes::None;
		if (Mode == EModes::Attack)
		{
			ActualPhysicAttack = DefaultPhysicAttack * 1.2f;
			ActualMagicAttack = DefaultMagicAttack * 1.2f;
			ActualPhysicDefense = DefaultPhysicDefense * 0.8f;
			ActualMagicDefense = DefaultMagicDefense * 0.8f;

			ActualFieldOfSight = DefaultFieldOfSight;
			ActualRange = DefaultRange;

			ActualSpeed = DefaultSpeed;
			GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;

			CurrentMode = Mode;
		}
		else if (Mode == EModes::Defense)
		{
			CurrentMode = Mode;

			ActualPhysicAttack = DefaultPhysicAttack;
			ActualMagicAttack = DefaultMagicAttack;
			ActualPhysicDefense = DefaultPhysicDefense * 1.6f;
			ActualMagicDefense = DefaultMagicDefense * 1.6f;

			ActualFieldOfSight = DefaultFieldOfSight;
			ActualRange = DefaultRange;

			ActualSpeed = 0.f;
			GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;

			ClearDestinations();
		}
		else if (Mode == EModes::Alert)
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
			GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;
		}
		else if (Mode == EModes::Movement)
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
			GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;

			CurrentMode = Mode;
		}
		else if (Mode == EModes::Invisible)
		{
			if (GetInvisibleCoolDown() <= 0.f)
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
				GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;

				CurrentMode = Mode;

				Multicast_SetInvisibleAsset(GetMode() == EModes::Invisible);
			}
		}
	}
}
void AUnit::ChangeMode(EModes Mode)
{
	if ((Mode != EModes::Invisible || GetInvisibleCoolDown() <= 0.f) && CurrentMode != Mode && Role == ROLE_Authority)
	{
		if (CurrentMode == EModes::Invisible)
		{
			InvisibleCoolDown = 180.f - InvisibleLimitedTime / 15.f * 180.f;
			InvisibleLimitedTime = 0.f;

			CurrentMode = EModes::None;

			Multicast_SetInvisibleAsset(GetMode() == EModes::Invisible);
		}

		WantedMode = Mode;
		CurrentMode = EModes::None;

		ActualPhysicAttack = 0;
		ActualMagicAttack = 0;
		ActualPhysicDefense = 0;
		ActualMagicDefense = 0;

		ActualFieldOfSight = 0;
		ActualRange = 0;

		ActualSpeed = 0.f;
		GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100;

		if (Player && Player->GetSkillsTree())
			PrepareChangingModeTime = 2.f / Player->GetSkillsTree()->GetModeChangeModifier();
		else
			PrepareChangingModeTime = 2.f;
	}
}
EModes AUnit::GetMode()
{
	return CurrentMode;
}
void AUnit::Multicast_SetIsMoving_Implementation(EAction NewAction)
{
	CurrentAction = NewAction;
}
void AUnit::ChangeLoopingAnimation(bool OverrideCheck)
{
	UAnimationAsset* NewAnimation;
	if (CurrentAction == EAction::Moving)
	{
		if (CurrentMode == EModes::Movement)
			NewAnimation = RunningAnimation;
		else
			NewAnimation = WalkingAnimation;
	}
	else if (CurrentAction == EAction::Attacking)
		NewAnimation = AttackingAnimation;
	else
	{
		if (CurrentMode == EModes::Defense)
			NewAnimation = ReadyIdleAnimation;
		else
			NewAnimation = NeutralIdleAnimation;
	}
	if (NewAnimation != CurrentAnimation || OverrideCheck)
	{
		CurrentAnimation = NewAnimation;
		GetMesh()->PlayAnimation(CurrentAnimation, true);
		if (CurrentAnimation == WalkingAnimation || CurrentAnimation == RunningAnimation)
		{
			GetMesh()->SetPlayRate(GetSpeed() * AdaptSpeed);
		}
		else
			GetMesh()->SetPlayRate(1.f);
	}
}
float AUnit::GetInvisibleCoolDown()
{
	if (Player->GetSkillsTree()->GetInvisibilityEnabled())
		return InvisibleCoolDown;
	else
		return 180.f;
}
float AUnit::GetInvisibleTime()
{
	return InvisibleLimitedTime;
}

//Visibility
bool AUnit::GetOpponentVisibility()
{
	return bVisibleForOpponent;
}
void AUnit::Multicast_SetVisibility_Implementation(bool bVisibility, FVector Position, FRotator Rotation)
{
	bVisibleForOpponent = bVisibility;
	if (Cast<AMousePlayerController>(GetWorld()->GetFirstPlayerController())->GetSide() != GetSide())
	{
		SetActorLocation(Position);
		SetActorRotation(Rotation);
		SetActorHiddenInGame(!bVisibility);
	}
}
void AUnit::Multicast_SetInvisibleAsset_Implementation(bool bInvisible)
{
	if (bInvisible)
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
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AUnit, SpecialTargetsActors, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, BoxSpecialTargetsActors, COND_Custom);
	DOREPLIFETIME_CONDITION(AUnit, Destinations, COND_Custom);

	DOREPLIFETIME(AUnit, Player);

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
	DOREPLIFETIME(AUnit, bVisibleForOpponent);
}
void AUnit::PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, SpecialTargetsActors, MySide == ESide::Red);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, BoxSpecialTargetsActors, MySide == ESide::Red);
	DOREPLIFETIME_ACTIVE_OVERRIDE(AUnit, Destinations, MySide == ESide::Red);
}