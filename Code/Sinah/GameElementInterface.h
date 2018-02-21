// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameElementInterface.generated.h"


UENUM(BlueprintType)
	enum class ESide : uint8
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

		//ESide
		virtual void SetSide(ESide NewSide) = 0;
		virtual ESide GetSide() = 0;

		//Attack
		virtual void ReceiveDamages(int Physic, int Magic, ESide AttackingSide) = 0;
		virtual float GetSize() = 0;

		//Statistics Getters
		virtual int GetMaxLife() = 0;
		virtual int GetCurrentLife() = 0;
		virtual float GetFieldOfSight() = 0;
		virtual float GetHalfHeight() = 0;
		virtual unsigned int GetLifeBarWidth() = 0;

		//Visibility
		virtual bool GetOpponentVisibility() = 0;
		virtual FVector GetLocation() = 0;

		//Dying
		virtual bool IsPendingKill() = 0;
};
