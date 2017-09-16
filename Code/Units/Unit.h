// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "../GameElementInterface.h"
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

		//Attack and heal
		virtual void Attack(const TScriptInterface<IGameElementInterface>& Target);
		virtual void ReceiveDamages(int Physic, int Magic, Side AttackingSide) override;
		virtual void Heal(int Heal);

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual int GetMaxLife() override;
		virtual int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
		virtual float GetSize() override;
		virtual int GetPhysicAttack();
		virtual int GetMagicAttack();
		virtual int GetPhysicDefense();
		virtual int GetMagicDefense();
		virtual float GetSpeed();
		virtual float GetRange();
		virtual unsigned int GetBuildingLevelRequired();

		virtual int GetFoodEatenInHalfASecond();
		virtual int GetCostInFood();
		virtual int GetCostInCells();
		virtual int GetCostInMetal();
		virtual int GetCostInCristals();

		virtual void SetSpeedMultiplicator(float Multiplicator);

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
			int CurrentLife;

		UPROPERTY(Replicated)
			int ActualMaxLife;
		UPROPERTY(Replicated)
			int ActualPhysicAttack;
		UPROPERTY(Replicated)
			int ActualMagicAttack;
		UPROPERTY(Replicated)
			int ActualPhysicDefense;
		UPROPERTY(Replicated)
			int ActualMagicDefense;
		UPROPERTY(Replicated)
			float ActualSpeed;
		UPROPERTY(Replicated)
			float SpeedMultiplicator;
		UPROPERTY(Replicated)
			float ActualFieldOfSight;
		UPROPERTY(Replicated)
			float ActualRange;

		UPROPERTY(Replicated)
			bool IsVisibleForOpponent;
		unsigned int BuildingLevelRequired;

		UPROPERTY(EditAnywhere)
			int DefaultMaxLife;
		UPROPERTY(EditAnywhere)
			int DefaultPhysicAttack;
		UPROPERTY(EditAnywhere)
			int DefaultMagicAttack;
		UPROPERTY(EditAnywhere)
			int DefaultPhysicDefense;
		UPROPERTY(EditAnywhere)
			int DefaultMagicDefense;
		UPROPERTY(EditAnywhere)
			float DefaultSpeed;
		UPROPERTY(EditAnywhere)
			float DefaultFieldOfSight;
		UPROPERTY(EditAnywhere)
			float DefaultRange;

		int FoodEatenInHalfASecond;

		int COST_IN_FOOD;
		int COST_IN_CELLS;
		int COST_IN_METAL;
		int COST_IN_CRISTALS;

		//Selection
		UPROPERTY(EditAnywhere)
			Side MySide;
		bool Selected;

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
