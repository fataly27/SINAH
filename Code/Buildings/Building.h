// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../GameElementInterface.h"
#include "Building.generated.h"

UCLASS(abstract)
class SINAH_API ABuilding : public AActor, public IGameElementInterface
{
	GENERATED_BODY()

	public:
		// Sets default values for this actor's properties
		ABuilding();
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		//Selection
		virtual void Select() override;
		virtual void Unselect() override;
		virtual bool IsSelected() override;

		//Side
		virtual Side GetSide() override;
		virtual void SetSide(Side NewSide) override;
		UFUNCTION(NetMulticast, Reliable)
			virtual void Multicast_SetSide(Side NewSide);

		//Attack
		virtual void ReceiveDamages(unsigned int Physic, unsigned int Magic, Side AttackingSide) override;

		//Heal
		virtual void Heal();

		//Level
		UFUNCTION(Server, Reliable, WithValidation)
			virtual void Server_LevelUp();
		virtual void SetLevel(unsigned int Level);

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual unsigned int GetMaxLife() override;
		virtual unsigned int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
		virtual float GetSize() override;
		virtual float GetHeal();
		virtual unsigned int GetLevel();
		virtual unsigned int GetMaxLevel();

		virtual unsigned int GetCostInFoodToLevel(unsigned int WantedLevel);
		virtual unsigned int GetCostInCellsToLevel(unsigned int WantedLevel);
		virtual unsigned int GetCostInMetalToLevel(unsigned int WantedLevel);
		virtual unsigned int GetCostInCristalsToLevel(unsigned int WantedLevel);

		//Visibility
		virtual bool GetOpponentVisibility() override;
		virtual FVector GetLocation();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:

		UStaticMeshComponent* BuildingMesh;
		UStaticMesh* StaticMesh;

		UMaterial* BuildingBlueMaterial;
		UMaterial* BuildingRedMaterial;
		UMaterial* BuildingNeutralMaterial;

		//Level
		unsigned int LevelMax;
		unsigned int CurrentLevel;
		TArray<unsigned int> COST_IN_FOOD;
		TArray<unsigned int> COST_IN_CELLS;
		TArray<unsigned int> COST_IN_METAL;
		TArray<unsigned int> COST_IN_CRISTALS;

		//Statistics
		UPROPERTY(Replicated)
			unsigned int CurrentLife;

		UPROPERTY(EditAnywhere)
			unsigned int DefaultMaxLife;
		UPROPERTY(EditAnywhere)
			unsigned int DefaultHeal;
		UPROPERTY(EditAnywhere)
			float DefaultFieldOfSight;

		UPROPERTY(Replicated)
			unsigned int ActualMaxLife;
		UPROPERTY(Replicated)
			unsigned int ActualHeal;
		UPROPERTY(Replicated)
			float ActualFieldOfSight;

		UPROPERTY(Replicated)
			bool IsVisibleForOpponent;

		float TimeSinceLastAttack;
		float TimeSinceLastHeal;

		//Selection
		bool Selected;
		UPROPERTY(EditAnywhere)
			Side MySide;
		UPROPERTY(EditAnywhere)
			UDecalComponent* SelectionMark;
		UMaterial* RedCircle;
		UMaterial* BlueCircle;
		UMaterial* NeutralCircle;
};
