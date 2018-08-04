// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "../GameElementInterface.h"
#include "../Widgets/GameBeforeStartingWidget.h"
#include "Unit.generated.h"

class AMultiplayerState;

UENUM(BlueprintType)
	enum class EModes : uint8
	{
		Attack,
		Defense,
		Alert,
		Movement,
		Invisible,
		None
	};
UENUM(BlueprintType)
	enum class EAction : uint8
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

		//Unit
		FName GetName();
		UTexture* GetUnitImage();

		//Selection
		virtual void Select() override;
		virtual void Unselect() override;
		virtual bool IsSelected() override;

		//ESide
		virtual ESide GetSide() override;
		virtual void SetSide(ESide NewSide, AMultiplayerState* NewPlayer) override;
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_SetSide(ESide NewSide, AMultiplayerState* NewPlayer);

		//Destinations
		void AddDestination(FVector Destination, FRotator Rotation);
		void ClearOneDestination();
		void ClearDestinations();
		TArray<FVector> GetDestinations();
		void Rotate();
		FVector GetLocationAfterAllMoves();

		//Target
		void AddSpecialTargets();
		void SetBoxSpecialTargets(TArray<TScriptInterface<IGameElementInterface>> NewTargets);
		void ClearSpecialTargets();
		TArray<TScriptInterface<IGameElementInterface>> GetSpecialTargets();
		void SetOpponentsInSight(TArray<TScriptInterface<IGameElementInterface>> Opponents);
		TArray<TScriptInterface<IGameElementInterface>> GetOpponentsInSight();
		void ClearOpponentsInSight();

		//Attack and heal
		void Attack(const TScriptInterface<IGameElementInterface>& Target);
		virtual void ReceiveDamages(int Physic, int Magic, ESide AttackingSide, AMultiplayerState* AttackingPlayer) override;
		UFUNCTION(NetMulticast, Unreliable)
			void Multicast_Spark(UParticleSystem* Particle);
		UFUNCTION(NetMulticast, Unreliable)
			void Multicast_Explosion();
		void Heal(int Heal);

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual int GetMaxLife() override;
		virtual int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
		virtual float GetSize() override;
		int GetPhysicAttack();
		int GetMagicAttack();
		int GetPhysicDefense();
		int GetMagicDefense();
		float GetSpeed();
		float GetRange();
		unsigned int GetBuildingLevelRequired();

		int GetFoodEatenInASecond();
		int GetCostInFood();
		int GetCostInCells();
		int GetCostInMetal();
		int GetCostInCristals();

		void SetSpeedMultiplicator(float Multiplicator);

		//EModes and animation
		void SetMode();
		void ChangeMode(EModes Mode);
		EModes GetMode();
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_SetIsMoving(EAction NewAction);
		void ChangeLoopingAnimation(bool OverrideCheck = false);

		float GetInvisibleCoolDown();
		float GetInvisibleTime();

		//Visibility
		virtual bool GetOpponentVisibility() override;
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_SetVisibility(bool bVisibility, FVector Position, FRotator Rotation);
		UFUNCTION(NetMulticast, Reliable)
			void Multicast_SetInvisibleAsset(bool bInvisible);
		virtual FVector GetLocation();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
		virtual void PreReplication(IRepChangedPropertyTracker &ChangedPropertyTracker);

	protected:
		//Unit
		FName Name;
		ECivs Civ;
		UTexture* UnitImage;
		float AdaptSpeed;
		float AdaptScale;

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
			bool bVisibleForOpponent;
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

		int FoodEatenInASecond;

		int CostInFood;
		int CostInCells;
		int CostInMetal;
		int CostInCristal;

		//Player
		UPROPERTY(Replicated)
			AMultiplayerState* Player;

		//Selection
		UPROPERTY(EditAnywhere, Replicated)
			ESide MySide;
		bool bSelected;

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

		//EModes and animation
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Enum)
			EModes CurrentMode;
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Enum)
			EModes WantedMode;
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Enum)
			EAction CurrentAction;

		UPROPERTY(Replicated)
			float InvisibleLimitedTime;
		UPROPERTY(Replicated)
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

		//Particles
		UParticleSystem* BlueSpark;
		UParticleSystem* RedSpark;
		UParticleSystem* Explosion;
};
