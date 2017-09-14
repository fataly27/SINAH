// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerHUD.h"
#include "MousePlayerController.generated.h"

class AMainCamera;
class AUnit;
class AMilitaryBuilding;

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

		Side GetSide();

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

		//EnemyView
		void SetOpponentView(bool Enable);
		bool IsOpponentViewEnabled();
		void EnableOpponentView();
		void DisableOpponentView();

		//Fog Of War functions
		void FogOfWar();
		void SetFogOfWarTexture(TArray<TScriptInterface<IGameElementInterface>> Actors);
		void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

		void ApplyZoneEffects(TArray<AMilitaryBuilding*> MilitaryBuildings, TArray<AUnit*> Units);

		//Costing functions
		void LevelUpBuilding(ABuilding* Building);
		void SpawnUnit(AMilitaryBuilding* Spawner, UClass* Unit);

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

		//Widget
		UFUNCTION(BlueprintImplementableEvent)
			void DisableLoading();
		UFUNCTION(BlueprintImplementableEvent)
			void SetText();
		UFUNCTION(BlueprintImplementableEvent)
			void EnableExit();

		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	protected:
		float TimeSinceLastHarvest;
		bool OpponentView;

		AMainCamera* MyPawn;
		TArray<TScriptInterface<IGameElementInterface>> ActorsSelected;
		TArray<TScriptInterface<IGameElementInterface>> ActorsSelectedByCurrentBox;

		APlayerHUD* HUD;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
			TypeBox BoxDisplayed;

		UPROPERTY(Replicated)
			Side PlayerSide;

		//Fog Of War
		UPROPERTY()
			UTexture2D* FogOfWarTexture;
		UMaterial* FogOfWarMaterial;
		UMaterialInstanceDynamic* DynFOWMaterial;
		FUpdateTextureRegion2D* TextureRegions;
		TArray<FColor> TextureData;
		const int32 MidTextureSize = 512;
		const float MidTerrainSize = 31700.f;

		//Widget
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
			TSubclassOf<class UUserWidget> wGameStartInfo;
		UPROPERTY(BlueprintReadWrite, Category = "Widgets")
			FString String;
		UPROPERTY(BlueprintReadWrite, Category = "Widgets")
			UUserWidget* MyGameStartInfo;

		FString OldString;
		bool IsThrobberEnabled;
		bool IsExitEnabled;
};
