// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerHUD.h"
#include "Units/Unit.h"
#include "MousePlayerController.generated.h"

class AMainCamera;
class AMilitaryBuilding;
class UMapWidget;
class UStatWidget;
class UUpWidget;
class UModesWidget;
class ULevelEconomicWidget;
class ULevelMilitaryWidget;
class USpawnWidget;
class USpawnEntityWidget;

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
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_LevelUpBuilding(ABuilding* Building);
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_SpawnUnit(AMilitaryBuilding* Spawner, TSubclassOf<AUnit> Unit);

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

		UFUNCTION(BlueprintImplementableEvent)
			void SetColorToBlue();
		UFUNCTION(BlueprintImplementableEvent)
			void SetColorToRed();

		//Modes
		UFUNCTION(BlueprintCallable)
			void SetModeToAttack();
		UFUNCTION(BlueprintCallable)
			void SetModeToDefense();
		UFUNCTION(BlueprintCallable)
			void SetModeToSpeed();
		UFUNCTION(BlueprintCallable)
			void SetModeToSight();
		UFUNCTION(BlueprintCallable)
			void SetModeToInvisible();
		void SetMode(Modes Mode);
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_ChangeMode(AUnit* Unit, Modes Mode);

		//LevelUp
		UFUNCTION(BlueprintCallable)
			void LevelUp();
		UFUNCTION(BlueprintCallable)
			void LevelUpZone(bool IsPlayer, bool IsLife, bool IsEffect);
		UFUNCTION(Server, Reliable, WithValidation)
			void Server_LevelUpZone(AMilitaryBuilding* MilitaryBuilding, bool IsPlayer, bool IsLife, bool IsEffect);

		//Spawn
		UFUNCTION(BlueprintCallable)
			void SpawnUnit(TSubclassOf<AUnit> Unit);
		USpawnEntityWidget* PrepareEntity(TSubclassOf<AUnit> UnitClass);

		UTexture* UnselectedTexture;
		UTexture* SelectedTexture;
		UTexture* SelectedAmidSeveralTexture;

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
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStartWidget")
			TSubclassOf<class UUserWidget> wGameStartInfo;
		UPROPERTY(BlueprintReadWrite, Category = "GameStartWidget")
			UUserWidget* MyGameStartInfo;

		UPROPERTY(BlueprintReadWrite, Category = "GameStartWidget")
			FString String;
		FString OldString;
		bool IsThrobberEnabled;
		bool IsExitEnabled;


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TopInterface")
			TSubclassOf<class UUpWidget> wTopInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "TopInterface")
			UUpWidget* MyTopInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatInterface")
			TSubclassOf<class UStatWidget> wStatInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "StatInterface")
			UStatWidget* MyStatInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapInterface")
			TSubclassOf<class UMapWidget> wMapInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "MapInterface")
			UMapWidget* MyMapInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModesInterface")
			TSubclassOf<class UModesWidget> wModesInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "ModesInterface")
			UModesWidget* MyModesInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EconomicLevelInterface")
			TSubclassOf<class ULevelEconomicWidget> wEconomicLevelInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "EconomicLevelInterface")
			ULevelEconomicWidget* MyEconomicLevelInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MilitaryLevelInterface")
			TSubclassOf<class ULevelMilitaryWidget> wMilitaryLevelInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "MilitaryLevelInterface")
			ULevelMilitaryWidget* MyMilitaryLevelInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnInterface")
			TSubclassOf<class USpawnWidget> wSpawnInterface;
		UPROPERTY(Replicated, BlueprintReadWrite, Category = "SpawnInterface")
			USpawnWidget* MySpawnInterface;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntityInterface")
			TSubclassOf<class USpawnEntityWidget> wSpawnEntityInterface;

		TArray<USpawnEntityWidget*> SpawnEntityWidgets;
};
