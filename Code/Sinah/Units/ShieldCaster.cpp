// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "ShieldCaster.h"

AShieldCaster::AShieldCaster() : Super()
{
	Name = FName(TEXT("Shield Caster"));

	DefaultMaxLife = 200;
	DefaultPhysicAttack = 0;
	DefaultMagicAttack = 1;
	DefaultPhysicDefense = 0;
	DefaultMagicDefense = 8;
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

	BuildingLevelRequired = 4;

	FoodEatenInASecond = 3;
	CostInFood = 50;
	CostInCells = 50;
	CostInMetal = 100;
	CostInCristal = 300;

	AdaptSpeed = 0.4f;
	AdaptScale = 0.8f;
	GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * AdaptSpeed;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAsset(TEXT("SkeletalMesh'/Game/Meshes/ShieldCaster/ShieldCaster.ShieldCaster'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PreVisualAssetTransparent(TEXT("SkeletalMesh'/Game/Meshes/ShieldCaster/ShieldCaster_Transparent.ShieldCaster_Transparent'"));
	VisualAsset = PreVisualAsset.Object;
	VisualAssetTransparent = PreVisualAssetTransparent.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Attacking(TEXT("AnimationAsset'/Game/Meshes/ShieldCaster/Animations/Attacking.Attacking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> NeutralIdle(TEXT("AnimationAsset'/Game/Meshes/ShieldCaster/Animations/NeutralIdle.NeutralIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> ReadyIdle(TEXT("AnimationAsset'/Game/Meshes/ShieldCaster/Animations/ReadyIdle.ReadyIdle'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Walking(TEXT("AnimationAsset'/Game/Meshes/ShieldCaster/Animations/Walking.Walking'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Running(TEXT("AnimationAsset'/Game/Meshes/ShieldCaster/Animations/Running.Running'"));
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> Victory(TEXT("AnimationAsset'/Game/Meshes/ShieldCaster/Animations/Victory.Victory'"));
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

	static ConstructorHelpers::FObjectFinder<UTexture> ImageAsset(TEXT("Texture'/Game/Textures/Units/ShieldCaster.ShieldCaster'"));
	UnitImage = ImageAsset.Object;
}