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

		//Color
		virtual void AmIBlue(bool color) override;
		virtual bool TellIfImBlue() override;

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
		virtual void ReceiveDamages(float Physic, float Magic) override;

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual int GetMaxLife() override;
		virtual int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual int GetLifeBarWidth() override;
		virtual float GetSize() override;
		virtual int GetPhysicAttack();
		virtual int GetMagicAttack();
		virtual int GetPhysicDefense();
		virtual int GetMagicDefense();
		virtual float GetSpeed();
		virtual float GetRange();
		virtual float GetReactionTime();

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
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
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
			float ActualFieldOfSight;
		UPROPERTY(Replicated)
			float ActualRange;
		UPROPERTY(Replicated)
			float ActualReactionTime;

		UPROPERTY(Replicated)
			bool IsVisibleForOpponent;

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
		UPROPERTY(Replicated)
			float DefaultReactionTime;

		UPROPERTY(EditAnywhere)
			int CostInFood;
		UPROPERTY(EditAnywhere)
			int CostInCells;
		UPROPERTY(EditAnywhere)
			int CostInMetal;
		UPROPERTY(EditAnywhere)
			int CostInCristals;

		//Selection
		UPROPERTY(EditAnywhere)
			bool ImBlue;
		bool Selected;
		UPROPERTY(EditAnywhere)
			UDecalComponent* SelectionMark;
		UMaterial* RedCircle;
		UMaterial* BlueCircle;

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
