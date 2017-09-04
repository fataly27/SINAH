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
		virtual void ReceiveDamages(int Physic, int Magic, Side AttackingSide) override;

		//Heal
		virtual void Heal();

		//Level
		UFUNCTION(Server, Reliable, WithValidation)
			virtual void Server_LevelUp();
		virtual void SetLevel(unsigned int Level);

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual int GetMaxLife() override;
		virtual int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual unsigned int GetLifeBarWidth() override;
		virtual float GetSize() override;
		virtual float GetHeal();
		virtual unsigned int GetLevel();
		virtual unsigned int GetMaxLevel();

		virtual int GetCostInFoodToLevel(int WantedLevel);
		virtual int GetCostInCellsToLevel(int WantedLevel);
		virtual int GetCostInMetalToLevel(int WantedLevel);
		virtual int GetCostInCristalsToLevel(int WantedLevel);

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
		TArray<int> COST_IN_FOOD;
		TArray<int> COST_IN_CELLS;
		TArray<int> COST_IN_METAL;
		TArray<int> COST_IN_CRISTALS;

		//Statistics
		UPROPERTY(Replicated)
			int CurrentLife;

		UPROPERTY(EditAnywhere)
			int DefaultMaxLife;
		UPROPERTY(EditAnywhere)
			int DefaultHeal;
		UPROPERTY(EditAnywhere)
			float DefaultFieldOfSight;

		UPROPERTY(Replicated)
			int ActualMaxLife;
		UPROPERTY(Replicated)
			int ActualHeal;
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
};
