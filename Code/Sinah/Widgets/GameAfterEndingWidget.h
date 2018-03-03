// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameBeforeStartingWidget.h"
#include "GameAfterEndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SINAH_API UGameAfterEndingWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintImplementableEvent)
			void SetWinCiv(const ECivs& Civ);
		UFUNCTION(BlueprintImplementableEvent)
			void SetLoseCiv(const ECivs& Civ);
		void SetDidWin(bool Win);

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "Win")
			FString Text;
};
