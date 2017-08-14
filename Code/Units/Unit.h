// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "UnitController.h"
#include "../GameElementInterface.h"
#include <cmath>
#include "Unit.generated.h"

UENUM(BlueprintType)
	enum class Modes : uint8
	{
		Attack,
		Defense,
		Alert,
		Movement,
		Invisible,
		None
	};
UENUM(BlueprintType)
	enum class Action : uint8
	{
		Idle,
		Moving,
		Attacking
	};

UCLASS(abstract)
class SINAH_API AUnit : public ACharacter, public IGameElementInterface
{
	GENERATED_BODY()

	public:
		// Sets default values for this character's properties
		AUnit();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;

		//Selection
		virtual void Select() override;
		virtual void Unselect() override;
		virtual bool IsSelected() override;

		//Side
		virtual Side GetSide() override;
		virtual void SetSide(Side NewSide) override;
		UFUNCTION(NetMulticast, Reliable)
			virtual void Multicast_SetSide(Side NewSide);

		//Destinations
		virtual void AddDestination(FVector Destination, FRotator Rotation);
		virtual void ClearOneDestination();
		virtual void ClearDestinations();
		virtual TArray<FVector> GetDestinations();
		virtual void Rotate();
		virtual FVector GetLocationAfterAllMoves();

		//Target
		virtual void AddSpecialTargets();
		virtual void SetBoxSpecialTargets(TArray<TScriptInterface<IGameElementInterface>> NewTargets);
		virtual void ClearSpecialTargets();
		virtual TArray<TScriptInterface<IGameElementInterface>> GetSpecialTargets();
		virtual void SetOpponentsInSight(TArray<TScriptInterface<IGameElementInterface>> Opponents);
		virtual TArray<TScriptInterface<IGameElementInterface>> GetOpponentsInSight();
		virtual void ClearOpponentsInSight();

		//Attack
		virtual void Attack(const TScriptInterface<IGameElementInterface>& Target);
		virtual void ReceiveDamages(float Physic, float Magic, Side AttackingSide) override;

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual unsigned int GetMaxLife() override;
		virtual unsigned int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
		virtual float GetSize() override;
		virtual unsigned int GetPhysicAttack();
		virtual unsigned int GetMagicAttack();
		virtual unsigned int GetPhysicDefense();
		virtual unsigned int GetMagicDefense();
		virtual float GetSpeed();
		virtual float GetRange();
		virtual unsigned int GetBuildingLevelRequired();

		unsigned int GetCostInFood();
		unsigned int GetCostInCells();
		unsigned int GetCostInMetal();
		unsigned int GetCostInCristals();

		//Modes and animation
		virtual void SetMode();
		UFUNCTION(Server, Reliable, WithValidation)
			virtual void Server_ChangeMode(Modes Mode);
		virtual Modes GetMode();
		UFUNCTION(NetMulticast, Reliable)
			virtual void Multicast_SetIsMoving(Action NewAction);
		virtual void ChangeLoopingAnimation();

		//Visibility
		virtual bool GetOpponentVisibility() override;
		UFUNCTION(NetMulticast, Reliable)
			virtual void Multicast_SetHidden(bool Hidden, FVector Position, FRotator Rotation, bool TurnIntoGhost = false);
		virtual FVector GetLocation();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
		virtual void PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker);

	protected:

		//Statistics
		UPROPERTY(Replicated)
			unsigned int CurrentLife;

		UPROPERTY(Replicated)
			unsigned int ActualMaxLife;
		UPROPERTY(Replicated)
			unsigned int ActualPhysicAttack;
		UPROPERTY(Replicated)
			unsigned int ActualMagicAttack;
		UPROPERTY(Replicated)
			unsigned int ActualPhysicDefense;
		UPROPERTY(Replicated)
			unsigned int ActualMagicDefense;
		UPROPERTY(Replicated)
			float ActualSpeed;
		UPROPERTY(Replicated)
			float ActualFieldOfSight;
		UPROPERTY(Replicated)
			float ActualRange;

		UPROPERTY(Replicated)
			bool IsVisibleForOpponent;
		unsigned int BuildingLevelRequired;

		UPROPERTY(EditAnywhere)
			unsigned int DefaultMaxLife;
		UPROPERTY(EditAnywhere)
			unsigned int DefaultPhysicAttack;
		UPROPERTY(EditAnywhere)
			unsigned int DefaultMagicAttack;
		UPROPERTY(EditAnywhere)
			unsigned int DefaultPhysicDefense;
		UPROPERTY(EditAnywhere)
			unsigned int DefaultMagicDefense;
		UPROPERTY(EditAnywhere)
			float DefaultSpeed;
		UPROPERTY(EditAnywhere)
			float DefaultFieldOfSight;
		UPROPERTY(EditAnywhere)
			float DefaultRange;

		unsigned int COST_IN_FOOD;
		unsigned int COST_IN_CELLS;
		unsigned int COST_IN_METAL;
		unsigned int COST_IN_CRISTALS;

		//Selection
		UPROPERTY(EditAnywhere)
			Side MySide;
		bool Selected;
		UPROPERTY(EditAnywhere)
			UDecalComponent* SelectionMark;
		UMaterial* RedCircle;
		UMaterial* BlueCircle;
		UMaterial* NeutralCircle;

		//Target
		UPROPERTY(Replicated)
			TArray<AActor*> SpecialTargetsActors;
		UPROPERTY(Replicated)
			TArray<AActor*> BoxSpecialTargetsActors;
		TArray<TScriptInterface<IGameElementInterface>> SpecialTargets;
		TArray<TScriptInterface<IGameElementInterface>> BoxSpecialTargets;

		TArray<TScriptInterface<IGameElementInterface>> OpponentsInSight;
		
		//Destinations
		FRotator DesiredRotation;
		UPROPERTY(Replicated)
			TArray<FVector> Destinations;

		//Modes and animation
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Enum)
			Modes CurrentMode;
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Enum)
			Modes WantedMode;
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Enum)
			Action CurrentAction;
		float InvisibleLimitedTime;
		float InvisibleCoolDown;
		float PrepareChangingModeTime;

		//Assets
		USkeletalMesh* VisualAsset;
		USkeletalMesh* VisualAssetTransparent;

		//Animations
		UAnimationAsset* AttackingAnimation;
		UAnimationAsset* NeutralIdleAnimation;
		UAnimationAsset* ReadyIdleAnimation;
		UAnimationAsset* WalkingAnimation;
		UAnimationAsset* RunningAnimation;
		UAnimationAsset* VictoryAnimation;

		UAnimationAsset* CurrentAnimation;
};
