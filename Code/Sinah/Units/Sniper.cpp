// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Sniper.h"

ASniper::ASniper() : Super()
{
	Name = FName(TEXT("Sniper"));

	DefaultMaxLife = 150;
	DefaultPhysicAttack = 8;
	DefaultMagicAttack = 0;
	DefaultPhysicDefense = 0;
	DefaultMagicDefense = 0;
	DefaultSpeed = 3.f;
	DefaultFieldOfSight = 11.f;
	DefaultRange = 10.f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualPhysicAttack = DefaultPhysicAttack;
	ActualMagicAttack = DefaultMagicAttack;
	ActualPhysicDefense = DefaultPhysicDefense;
	ActualMagicDefense = DefaultMagicDefense;
	ActualSpeed = DefaultSpeed;
	ActualFieldOfSight = DefaultFieldOfSight;
	ActualRange = DefaultRange;

	BuildingLevelRequired = 3;

	FoodEatenInASecond = 2;
	CostInFood = 50;
	CostInCells = 250;
	CostInMetal = 50;
	CostInCristal = 0;

	AdaptSpeed = 0.3f;
	AdaptScale = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * AdaptSpeed;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAsset(TEXT("SkeletalMesh'/Game/Meshes/Sniper/swat.swat'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAssetTransparent(TEXT("SkeletalMesh'/Game/Meshes/Sniper/swat_Transparent.swat_Transparent'"));
	VisualAsset = PreVisualAsset.Object;
	VisualAssetTransparent = PreVisualAssetTransparent.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Attacking(TEXT("AnimationAsset'/Game/Meshes/Sniper/Animations/Attacking.Attacking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> NeutralIdle(TEXT("AnimationAsset'/Game/Meshes/Sniper/Animations/NeutralIdle.NeutralIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ReadyIdle(TEXT("AnimationAsset'/Game/Meshes/Sniper/Animations/ReadyIdle.ReadyIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Walking(TEXT("AnimationAsset'/Game/Meshes/Sniper/Animations/Walking.Walking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Running(TEXT("AnimationAsset'/Game/Meshes/Sniper/Animations/Running.Running'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Victory(TEXT("AnimationAsset'/Game/Meshes/Sniper/Animations/Victory.Victory'"));
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

	static ConstructorHelpers::FObjectFinder<UTexture> ImageAsset(TEXT("Texture'/Game/Textures/Units/Sniper.Sniper'"));
	UnitImage = ImageAsset.Object;
}