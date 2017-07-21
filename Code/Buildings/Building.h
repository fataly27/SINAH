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

	//Color
	virtual void AmIBlue(bool color) override;
	virtual bool TellIfImBlue() override;

	//Attack
	virtual void ReceiveDamages(float Physic, float Magic) override;

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
	virtual int GetLifeBarWidth() override;
	virtual float GetSize() override;
	virtual float GetHeal();

	//Visibility
	virtual bool GetOpponentVisibility() override;
	virtual FVector GetLocation();

	//Replication
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

protected:

	UStaticMeshComponent* BuildingMesh;
	UStaticMesh* StaticBlueMesh;
	UStaticMesh* StaticRedMesh;

	//Level
	unsigned int LevelMax;
	unsigned int CurrentLevel;
	TArray<int> CostInFood;
	TArray<int> CostInCells;
	TArray<int> CostInMetal;
	TArray<int> CostInCristals;

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
		bool ImBlue;
	UPROPERTY(EditAnywhere)
		UDecalComponent* SelectionMark;
	UMaterial* RedCircle;
	UMaterial* BlueCircle;
};
