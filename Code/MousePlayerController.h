// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MainCamera.h"
#include "Units/Unit.h"
#include "Buildings/Building.h"
#include "PlayerHUD.h"
#include "GameElementInterface.h"
#include "Core.h"
#include <cmath>
#include <algorithm>
#include "MousePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public:
		AMousePlayerController();

		// Called to bind functionality to input
		virtual void SetupInputComponent();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// Called every frame
		virtual void Tick(float DeltaSeconds) override;

		virtual void SetPawn(APawn* InPawn) override;

		//Input functions
		void MoveForward(float AxisValue);
		void MoveRight(float AxisValue);
		void Zoom(float AxisValue);

		void StartSelect();
		void StartAddSelect();
		void Select();
		void AddSelect();
		void StartDirect();
		void StartAddDirect();
		void Direct();
		void AddDirect();

		//Selection functions
		void ClearSelection();
		void UpdateBoxSelection(TArray<TScriptInterface<IGameElementInterface>> NewSelection);
		void UpdateBoxTargeting(TArray<TScriptInterface<IGameElementInterface>> NewTargets, bool IsFinal);

		//Fog Of War functions
		void FogOfWar();
		void SetFogOfWarTexture(TArray<TScriptInterface<IGameElementInterface>> BlueUnits, TArray<TScriptInterface<IGameElementInterface>> RedUnits);
		void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

		UFUNCTION(Server, Reliable, WithValidation)
			void Server_AddDestination(AUnit *Unit, FVector Destination, FRotator Rotation);
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_ClearDestinations(AUnit *Unit);

		UFUNCTION(Server, Reliable, WithValidation)
			void Server_AddSpecialTargets(AUnit *Unit);
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_SetBoxSpecialTargets(AUnit *Unit, const TArray<AActor*> &NewTargets);
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_ClearSpecialTargets(AUnit *Unit);


	protected:
		AMainCamera* MyPawn;
		TArray<TScriptInterface<IGameElementInterface>> ActorsSelected;
		TArray<TScriptInterface<IGameElementInterface>> ActorsSelectedByCurrentBox;

		APlayerHUD* HUD;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
			TypeBox BoxDisplayed;

		bool PlayerIsBlue;

		//Fog Of War
		UPROPERTY()
			UTexture2D* FogOfWarTexture;
		UMaterial* FogOfWarMaterial;
		UMaterialInstanceDynamic* DynFOWMaterial;
		FUpdateTextureRegion2D* TextureRegions;
		TArray<FColor> TextureData;
		const int32 MidTextureSize = 512;
		const float MidTerrainSize = 31700.f;
};
