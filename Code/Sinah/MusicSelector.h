// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "MusicSelector.generated.h"

class UMusicItem;
class UUpWidget;

/**
 * 
 */
UCLASS()
class SINAH_API UMusicSelector : public UAudioComponent
{
	GENERATED_BODY()

	public:
		// Sets default values for this character's properties
		UMusicSelector();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// Called every frame
		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

		void InitCivs(ECivs Player, ECivs Opponent);
		void SetTopWidget(UUpWidget* Widget);

		void Skip();

	protected:
		UPROPERTY()
			TArray<UMusicItem*> Musics;
		float SilenceTime;
		ECivs PlayerCiv;
		ECivs OpponentCiv;

		UUpWidget* UpWidget;
};
