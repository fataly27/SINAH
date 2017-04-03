// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "Units/Unit.h"
#include "Buildings/Building.h"
#include "GameElementInterface.h"
#include <cmath>
#include "PlayerHUD.generated.h"

UENUM(BlueprintType)
	enum class TypeBox : uint8
	{
		Select,
		Target,
		None
	};

UCLASS()
class SINAH_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
	public:
		APlayerHUD();

		virtual void BeginPlay() override;
		virtual void DrawHUD() override;

		void SetStartMousePos(FVector2D Pos);
		void SetCurrentMousePos(FVector2D Pos);
		void ShouldDisplayBox(TypeBox Display);
		void IsPlayerBlue(bool color);
		bool IsBoxTiny();

		void SetActorsSelected(TArray<TScriptInterface<IGameElementInterface>> Units);
		TArray<TScriptInterface<IGameElementInterface>> GetActorsBeingSelected();

		void SetZoom(float Zoom);

	protected:
		FVector2D StartMousePos;
		FVector2D CurrentMousePos;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
			TypeBox BoxDisplayed;
		bool PlayerIsBlue;
		TArray<TScriptInterface<IGameElementInterface>> ActorsSelected;
		TArray<TScriptInterface<IGameElementInterface>> ActorsBeingSelected;
		
		UTexture* BlueDestinationTexture;
		UTexture* RedDestinationTexture;
		UTexture* BlueTargetTexture;
		UTexture* RedTargetTexture;
		UTexture* BlueTargetTextureLight;
		UTexture* RedTargetTextureLight;

		float CurrentZoom;
};
