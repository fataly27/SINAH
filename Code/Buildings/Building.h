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

		//Dying
		virtual bool IsPendingKill();

		//Statistics Getters
		virtual int GetMaxLife() override;
		virtual int GetCurrentLife() override;
		virtual float GetFieldOfSight() override;
		virtual float GetHalfHeight() override;
		virtual float GetHeal();

		//Visibility
		virtual bool GetOpponentVisibility() override;
		virtual FVector GetLocation();

		//Replication
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

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

		//Selection
		bool Selected;
		UPROPERTY(EditAnywhere)
			bool ImBlue;
		UPROPERTY(EditAnywhere)
			UDecalComponent* SelectionMark;
		UMaterial* RedCircle;
		UMaterial* BlueCircle;
};
