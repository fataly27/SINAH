// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Tank.h"

ATank::ATank() : Super()
{
	Name = FName(TEXT("Tank"));

	DefaultMaxLife = 600;
	DefaultPhysicAttack = 3;
	DefaultMagicAttack = 0;
	DefaultPhysicDefense = 0;
	DefaultMagicDefense = 0;
	DefaultSpeed = 1.5f;
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

	FoodEatenInASecond = 3;
	CostInFood = 150;
	CostInCells = 100;
	CostInMetal = 200;
	CostInCristal = 0;

	AdaptSpeed = 0.8f;
	AdaptScale = 0.1f;
	GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * AdaptSpeed;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAsset(TEXT("SkeletalMesh'/Game/Meshes/Tank/Tank.Tank'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAssetTransparent(TEXT("SkeletalMesh'/Game/Meshes/Tank/Tank_Transparent.Tank_Transparent'"));
	VisualAsset = PreVisualAsset.Object;
	VisualAssetTransparent = PreVisualAssetTransparent.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Attacking(TEXT("AnimationAsset'/Game/Meshes/Tank/Animations/Attacking.Attacking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> NeutralIdle(TEXT("AnimationAsset'/Game/Meshes/Tank/Animations/NeutralIdle.NeutralIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ReadyIdle(TEXT("AnimationAsset'/Game/Meshes/Tank/Animations/ReadyIdle.ReadyIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Walking(TEXT("AnimationAsset'/Game/Meshes/Tank/Animations/Walking.Walking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Running(TEXT("AnimationAsset'/Game/Meshes/Tank/Animations/Running.Running'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Victory(TEXT("AnimationAsset'/Game/Meshes/Tank/Animations/Victory.Victory'"));
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
		GetMesh()->SetRelativeScale3D(FVector(AdaptScale));
	}

	GetCapsuleComponent()->SetCapsuleRadius(50.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.f));

	static ConstructorHelpers::FObjectFinder<UTexture> ImageAsset(TEXT("Texture'/Game/Textures/Units/Tank.Tank'"));
	UnitImage = ImageAsset.Object;
}