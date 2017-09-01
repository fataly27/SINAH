// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MainCamera.h"


// Sets default values
AMainCamera::AMainCamera() : ZoomMin(15.f), ZoomMax(45.f), XMin(-50.f), XMax(40.f), YMin(-45.f), YMax(45.f), DefaultZoom(33.f), Angle(40.f), MoveScale(300.f), ZoomScale(100.f), CurrentLocation(0.f, 0.f, 0.f), Right(0.f, 0.f, 0.f), Forward(0.f, 0.f, 0.f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(RootComponent);

	if (ZoomMin > ZoomMax)
		ZoomMin = ZoomMax;
	if (DefaultZoom < ZoomMin)
		DefaultZoom = ZoomMin;
	if (DefaultZoom > ZoomMax)
		DefaultZoom = ZoomMax;

	CurrentZoom = DefaultZoom;
}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();

	FRotator Rotation;

	if (Role == ROLE_Authority)
	{
		Right.Y = 1.0f;
		Forward.X = 1.0f;
		Rotation = FRotator(-90.0f + Angle, 0.0f, 0.0f);
	}
	else
	{
		Right.Y = -1.0f;
		Forward.X = -1.0f;
		Rotation = FRotator(-90.0f - Angle, 0.0f, 180.0f);
	}

	this->SetActorRotation(Rotation);
	this->SetActorLocation(-GetActorForwardVector() * CurrentZoom * ZoomScale);
}

// Called every frame
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RelativeZoom != 0)
	{
		RelativeZoom *= ZoomScale * DeltaTime;
		CurrentZoom += RelativeZoom;

		if (CurrentZoom > ZoomMax)
			CurrentZoom = ZoomMax;
		else if (CurrentZoom < ZoomMin)
			CurrentZoom = ZoomMin;
	}
	if (!MovementInput.IsZero())
	{
		MovementInput = MovementInput.GetSafeNormal() * MoveScale * DeltaTime * CurrentZoom;

		CurrentLocation += Forward * MovementInput.X;
		CurrentLocation += Right * MovementInput.Y;

		if (CurrentLocation.X > XMax * MoveScale)
			CurrentLocation.X = XMax * MoveScale;
		else if (CurrentLocation.X < XMin * MoveScale)
			CurrentLocation.X = XMin * MoveScale;
		if (CurrentLocation.Y > YMax * MoveScale)
			CurrentLocation.Y = YMax * MoveScale;
		else if (CurrentLocation.Y < YMin * MoveScale)
			CurrentLocation.Y = YMin * MoveScale;
	}

	SetActorLocation(CurrentLocation - GetActorForwardVector() * CurrentZoom * ZoomScale);
}


//Setters
void AMainCamera::SetMovementInputX(float MovementInputX)
{
	MovementInput.X = FMath::Clamp<float>(MovementInputX, -1.0f, 1.0f);
}
void AMainCamera::SetMovementInputY(float MovementInputY)
{
	MovementInput.Y = FMath::Clamp<float>(MovementInputY, -1.0f, 1.0f);
}
void AMainCamera::SetZoom(float Zoom)
{
	RelativeZoom = FMath::Clamp<float>(Zoom, -1.0f, 1.0f);
}

//Getters
float AMainCamera::GetZoom()
{
	return CurrentZoom;
}