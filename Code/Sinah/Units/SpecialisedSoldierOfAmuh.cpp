// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "SpecialisedSoldierOfAmuh.h"

ASpecialisedSoldierOfAmuh::ASpecialisedSoldierOfAmuh() : Super()
{
	Name = FName(TEXT("Specialised soldier of Amuh"));

	DefaultMaxLife = 200;
	DefaultPhysicAttack = 10;
	DefaultMagicAttack = 0;
	DefaultPhysicDefense = 0;
	DefaultMagicDefense = 0;
	DefaultSpeed = 2.f;
	DefaultFieldOfSight = 5.f;
	DefaultRange = 1.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualPhysicAttack = DefaultPhysicAttack;
	ActualMagicAttack = DefaultMagicAttack;
	ActualPhysicDefense = DefaultPhysicDefense;
	ActualMagicDefense = DefaultMagicDefense;
	ActualSpeed = DefaultSpeed;
	ActualFieldOfSight = DefaultFieldOfSight;
	ActualRange = DefaultRange;

	BuildingLevelRequired = 2;

	FoodEatenInASecond = 1;
	CostInFood = 5;
	CostInCells = 60;
	CostInMetal = 40;
	CostInCristal = 20;

	GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;

	SetActorScale3D(FVector(0.8f));

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAsset(TEXT("SkeletalMesh'/Game/Meshes/Paladin/Paladin.Paladin'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAssetTransparent(TEXT("SkeletalMesh'/Game/Meshes/Paladin/Paladin_Transparent.Paladin_Transparent'"));
	VisualAsset = PreVisualAsset.Object;
	VisualAssetTransparent = PreVisualAssetTransparent.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Attacking(TEXT("AnimationAsset'/Game/Meshes/Paladin/Animations/Attacking.Attacking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> NeutralIdle(TEXT("AnimationAsset'/Game/Meshes/Paladin/Animations/NeutralIdle.NeutralIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ReadyIdle(TEXT("AnimationAsset'/Game/Meshes/Paladin/Animations/ReadyIdle.ReadyIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Walking(TEXT("AnimationAsset'/Game/Meshes/Paladin/Animations/Walking.Walking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Running(TEXT("AnimationAsset'/Game/Meshes/Paladin/Animations/Running.Running'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Victory(TEXT("AnimationAsset'/Game/Meshes/Paladin/Animations/Victory.Victory'"));
	AttackingAnimation = Attacking.Object;
	NeutralIdleAnimation = NeutralIdle.Object;
	ReadyIdleAnimation = ReadyIdle.Object;
	WalkingAnimation = Walking.Object;
	RunningAnimation = Running.Object;
	VictoryAnimation = Victory.Object;

	if (PreVisualAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(VisualAsset);
		GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	}

	GetCapsuleComponent()->SetCapsuleRadius(50.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.f));

	static ConstructorHelpers::FObjectFinder<UTexture> ImageAsset(TEXT("Texture'/Game/Textures/Units/Paladin.Paladin'"));
	UnitImage = ImageAsset.Object;*/
}

float ASpecialisedSoldierOfAmuh::GetSize()
{
	return 28.f;
}
unsigned int ASpecialisedSoldierOfAmuh::GetLifeBarWidth()
{
	return 500;
}