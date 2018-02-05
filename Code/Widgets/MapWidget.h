// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintImplementableEvent)
			void SetFogForMap(const UTexture2D* Texture);
		UFUNCTION(BlueprintImplementableEvent)
			void SetMapForMap(const UTexture2D* Texture);
		UFUNCTION(BlueprintImplementableEvent)
			void SetColorForIndex(int32 index, const FString& Color);
		UFUNCTION(BlueprintImplementableEvent)
			void Rotate();
};
