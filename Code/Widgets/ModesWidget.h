// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModesWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API UModesWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		void SetModesVisibility(ESlateVisibility Unit);
		void SetAllButtonsEnabled(bool bAll);
		void SetInvisibleButtonEnabled(bool bInvisible);

		UFUNCTION(BlueprintImplementableEvent, Category = "Invisible")
			void SetInvisibleButtonProgress(const float Progress);

		UFUNCTION(BlueprintImplementableEvent)
			void SetButtonsTextures(const UTexture* Attack, const UTexture* Defense, const UTexture* Speed, const UTexture* Sight, const UTexture* Invisible);

		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
	
	protected:
		UPROPERTY(BlueprintReadWrite, Category = "EModes")
			ESlateVisibility ModesVisibility;

		UPROPERTY(BlueprintReadWrite, Category = "Invisible")
			bool InvisibleButtonEnabled;
		UPROPERTY(BlueprintReadWrite, Category = "EModes")
			bool OtherButtonsEnabled;
};
