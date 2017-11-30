// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MainCamera.h"


// Sets default values
AMainCamera::AMainCamera() : ZoomMin(15.f), ZoomMax(45.f), XMin(-60.f), XMax(50.f), YMin(-55.f), YMax(55.f), DefaultZoom(33.f), Angle(40.f), MoveScale(250.f), ZoomScale(100.f), CurrentLocation(0.f, 0.f, 0.f), Right(0.f, 0.f, 0.f), Forward(0.f, 0.f, 0.f)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->PostProcessSettings.AutoExposureMinBrightness = 1.f;
	OurCamera->PostProcessSettings.AutoExposureMaxBrightness = 1.f;

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
	FVector Location = -GetActorForwardVector() * CurrentZoom * ZoomScale;

	if (Role == ROLE_Authority)
	{
		Right.X = 1.0f;
		Right.Y = 1.0f;
		Forward.X = 1.0f;
		Forward.Y = -1.0f;
		Rotation = FRotator(-90.0f + Angle, -45.f, 0.f);

		CurrentLocation.X -= 11000.f;
		CurrentLocation.Y += 11000.f;
	}
	else
	{
		Right.X = -1.0f;
		Right.Y = -1.0f;
		Forward.X = -1.0f;
		Forward.Y = 1.0f;
		Rotation = FRotator(-90.0f - Angle, -45.f, 180.f);

		CurrentLocation.X += 11000.f;
		CurrentLocation.Y -= 11000.f;
	}

	SetActorRotation(Rotation);
	SetActorLocation(CurrentLocation - GetActorForwardVector() * CurrentZoom * ZoomScale);
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