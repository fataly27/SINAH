// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameElementInterface.generated.h"


UENUM(BlueprintType)
	enum class Side : uint8
	{
		Blue,
		Red,
		Neutral
	};

// This class does not need to be modified.
UINTERFACE()
class UGameElementInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class SINAH_API IGameElementInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		//Selection
		virtual void Select() = 0;
		virtual void Unselect() = 0;
		virtual bool IsSelected() = 0;

		//Side
		virtual void SetSide(Side NewSide) = 0;
		virtual Side GetSide() = 0;

		//Attack
		virtual void ReceiveDamages(float Physic, float Magic, Side AttackingSide) = 0;
		virtual float GetSize() = 0;

		//Statistics Getters
		virtual unsigned int GetMaxLife() = 0;
		virtual unsigned int GetCurrentLife() = 0;
		virtual float GetFieldOfSight() = 0;
		virtual float GetHalfHeight() = 0;
		virtual unsigned int GetLifeBarWidth() = 0;

		//Visibility
		virtual bool GetOpponentVisibility() = 0;
		virtual FVector GetLocation() = 0;

		//Dying
		virtual bool IsPendingKill() = 0;
};
