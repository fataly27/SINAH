// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MainCamera.generated.h"

UCLASS()
class SINAH_API AMainCamera : public APawn
{
	GENERATED_BODY()

	public:
		// Sets default values for this pawn's properties
		AMainCamera();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;

		//Setters
		void SetMovementInputX(float AxisValue);
		void SetMovementInputY(float AxisValue);
		void SetZoom(float AxisValue);

		//Getters
		float GetZoom();
	
	private:

		//Input variables
		FVector2D MovementInput;
		float RelativeZoom;
		float CurrentZoom;
		FVector CurrentLocation;

		FVector Right;
		FVector Forward;

		UPROPERTY(EditAnywhere)
			float ZoomMin;
		UPROPERTY(EditAnywhere)
			float ZoomMax;
		UPROPERTY(EditAnywhere)
			float XMin;
		UPROPERTY(EditAnywhere)
			float XMax;
		UPROPERTY(EditAnywhere)
			float YMin;
		UPROPERTY(EditAnywhere)
			float YMax;
		UPROPERTY(EditAnywhere)
			float DefaultZoom;
		UPROPERTY(EditAnywhere)
			float Angle;
		UPROPERTY(EditAnywhere)
			float MoveScale;
		UPROPERTY(EditAnywhere)
			float ZoomScale;
};
