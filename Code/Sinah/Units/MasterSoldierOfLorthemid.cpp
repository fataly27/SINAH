// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MasterSoldierOfLorthemid.h"

AMasterSoldierOfLorthemid::AMasterSoldierOfLorthemid() : Super()
{
	Name = FName(TEXT("Master soldier of Lorthemid"));

	DefaultMaxLife = 400;
	DefaultPhysicAttack = 7;
	DefaultMagicAttack = 7;
	DefaultPhysicDefense = 7;
	DefaultMagicDefense = 4;
	DefaultSpeed = 4.f;
	DefaultFieldOfSight = 8.f;
	DefaultRange = 4.f;

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

	FoodEatenInASecond = 4;
	CostInFood = 150;
	CostInCells = 200;
	CostInMetal = 200;
	CostInCristal = 300;

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

float AMasterSoldierOfLorthemid::GetSize()
{
	return 28.f;
}
unsigned int AMasterSoldierOfLorthemid::GetLifeBarWidth()
{
	return 500;
}