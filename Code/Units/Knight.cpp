// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "Knight.h"

AKnight::AKnight() : Super()
{
	DefaultMaxLife = 300;
	DefaultPhysicAttack = 20;
	DefaultMagicAttack = 0;
	DefaultPhysicDefense = 12;
	DefaultMagicDefense = 0;
	DefaultSpeed = 3.5f;
	DefaultFieldOfSight = 15.f;
	DefaultRange = 2.f;
	DefaultReactionTime = 0.8f;

	ActualMaxLife = DefaultMaxLife;
	CurrentLife = ActualMaxLife;
	ActualPhysicAttack = DefaultPhysicAttack;
	ActualMagicAttack = DefaultMagicAttack;
	ActualPhysicDefense = DefaultPhysicDefense;
	ActualMagicDefense = DefaultMagicDefense;
	ActualSpeed = DefaultSpeed;
	ActualFieldOfSight = DefaultFieldOfSight;
	ActualRange = DefaultRange;
	ActualReactionTime = DefaultReactionTime;

	CostInFood = 50;
	CostInCells = 0;
	CostInMetal = 15;
	CostInCristals = 0;

	GetCharacterMovement()->MaxWalkSpeed = ActualSpeed * 100;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnightVisualAsset(TEXT("SkeletalMesh'/Game/Meshes/Paladin/Paladin.Paladin'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> KnightVisualAssetTransparent(TEXT("SkeletalMesh'/Game/Meshes/Paladin/Paladin_Transparent.Paladin_Transparent'"));
	VisualAsset = KnightVisualAsset.Object;
	VisualAssetTransparent = KnightVisualAssetTransparent.Object;

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

	if (KnightVisualAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(VisualAsset);
		GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	}

	GetCapsuleComponent()->SetCapsuleRadius(50.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.f));
	SelectionMark->SetWorldScale3D(FVector(2.f, 0.3f, 0.3f));
}

float AKnight::GetSize()
{
	return 20.f;
}
int AKnight::GetLifeBarWidth()
{
	return 500;
}