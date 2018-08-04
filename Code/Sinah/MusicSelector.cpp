// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MousePlayerController.h"
#include "MusicItem.h"
#include "MusicSelector.h"
#include "Widgets/UpWidget.h"
#include "SoundProcessingLibrary.h"


UMusicSelector::UMusicSelector() : Super(), SilenceTime(0.f)
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;

	FString MusicsPath = FPaths::ProjectContentDir() + "/Musics/";

	UMusicItem* Item = NewObject<UMusicItem>();
	Item->Init(90, 80, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "A_Kingdom_Rises.ogg"), "A Kingdom Rises");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(50, 40, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Aduro.ogg"), "Aduro");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 15, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Awakening.ogg"), "Awakening");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 80, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Battle_of_Kings.ogg"), "Battle of Kings");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(40, 50, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Battle_of_the_Titans.ogg"), "Battle of the Titans");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 60, TArray<ECivs>({ ECivs::Hovanion, ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Beyond_the_Stars.ogg"), "Beyond the Stars");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(80, 90, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Champions_of_Tomorrow.ogg"), "Champions of Tomorrow");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 70, TArray<ECivs>({ ECivs::Hovanion, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Cloister_of_Redemption.ogg"), "Cloister of Redemption");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 80, TArray<ECivs>({ ECivs::Hovanion, ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Data_Mountains_Flyby.ogg"), "Data Mountains Flyby");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(70, 70, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Distant_Shores.ogg"), "Distant Shores");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(50, 50, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Dustsucker.ogg"), "Dustsucker");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 10, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Ecstatic_Wave.ogg"), "Ecstatic Wave");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 60, TArray<ECivs>({ ECivs::Lorthemid, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "End_Game.ogg"), "End Game");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(70, 80, TArray<ECivs>({ ECivs::Lorthemid, ECivs::Amuh }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Ephemeral_Reign.ogg"), "Ephemeral Reign");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(50, 70, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Escape_from_the_Temple.ogg"), "Escape from the Temple");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(25, 15, TArray<ECivs>({ ECivs::Hovanion, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Fields_of_Green.ogg"), "Fields of Green");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 60, TArray<ECivs>({ ECivs::Hovanion, ECivs::Lorthemid}), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Final_Hour.ogg"), "Final Hour");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 70, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Fire_Within.ogg"), "Fire Within");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(10, 15, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Galactic_Exploration.ogg"), "Galactic Exploration");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 25, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Going_Home.ogg"), "Going Home");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(70, 60, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Leap_of_Faith.ogg"), "Leap of Faith");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(90, 90, TArray<ECivs>({ ECivs::Hovanion, ECivs::Lorthemid, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Mars_Colony_Theta.ogg"), "Mars Colony Theta");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(70, 70, TArray<ECivs>({ ECivs::Amuh, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Queen_Of_The_Night.ogg"), "Queen of the Night");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 90, TArray<ECivs>({ ECivs::Hovanion, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Rallying_the_Defense.ogg"), "Rallying the Defense");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(50, 80, TArray<ECivs>({ ECivs::Amuh, ECivs::Zhark, ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Rebirth.ogg"), "Rebirth");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(40, 50, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Replicant_Citadel.ogg"), "Replicant Citadel");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(60, 50, TArray<ECivs>({ ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Setting_Sail.ogg"), "Setting Sail");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(90, 90, TArray<ECivs>({ ECivs::Zhark, ECivs::Hovanion, ECivs::Lorthemid, ECivs::Amuh }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Sinah.ogg"), "Sinah");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(90, 90, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Skirt_Shaker.ogg"), "Skirt Shaker");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(20, 90, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Streets_of_the_Unknown.ogg"), "Street of the Unknown");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 10, TArray<ECivs>({ ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Summer_Rain.ogg"), "Summer Rain");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(20, 90, TArray<ECivs>({ ECivs::Lorthemid, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "The_Grid.ogg"), "The Grid");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(15, 90, TArray<ECivs>({ ECivs::Hovanion, ECivs::Zhark }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "The_Hyperborean_Menace.ogg"), "The Hyperborean Menace");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(20, 20, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "The_Land_of_the_Wizard.ogg"), "The Land of the Wizard");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(40, 55, TArray<ECivs>({ ECivs::Hovanion }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Towards_the_Gates_of_Hell.ogg"), "Towards the Gates of Hell");
	Musics.Add(Item);
	//
	Item = NewObject<UMusicItem>();
	Item->Init(40, 50, TArray<ECivs>({ ECivs::Lorthemid }), USoundProcessingLibrary::LoadOggFile(MusicsPath + "Twilight_Dew.ogg"), "Twilight Dew");
	Musics.Add(Item);
}

void UMusicSelector::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(true);
	SetComponentTickInterval(1.f);
}

void UMusicSelector::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SilenceTime -= DeltaTime;
	if (!IsPlaying() && Cast<AMousePlayerController>(GetOwner()) && SilenceTime <= 0.f && PlayerCiv != ECivs::None && OpponentCiv != ECivs::None)
	{
		if (FMath::RandRange(0.f, 1.f) <= 0.2f)
		{
			SilenceTime = 60 * FMath::RandRange(1.f, 3.f);

			UpWidget->SetMusicTitle("");
			UpWidget->SetMusicCivs(TArray<ECivs>());
		}
		else
		{
			int VictoryPercents = Cast<AMousePlayerController>(GetOwner())->GetVictoryPercents();
			int EpicnessPercents = Cast<AMousePlayerController>(GetOwner())->GetEpicnessPercents();

			if (VictoryPercents != 0 && EpicnessPercents != 0)
			{
				int TotalChance = 0;
				for (int i(0); i < Musics.Num(); i++)
				{
					TotalChance += FMath::Square(Musics[i]->GetPercents(VictoryPercents, EpicnessPercents, PlayerCiv, OpponentCiv));
					Musics[i]->TurnPass();
				}
				int Chance = FMath::RandRange(0, TotalChance);
				for (int i(0); i < Musics.Num(); i++)
				{
					Chance -= FMath::Square(Musics[i]->GetPercents(VictoryPercents, EpicnessPercents, PlayerCiv, OpponentCiv));
					if (Chance <= 0)
					{
						SetSound(Musics[i]->Play());
						Play();
						UpWidget->SetMusicTitle(Musics[i]->GetTitle());
						UpWidget->SetMusicCivs(Musics[i]->GetCivs());
						break;
					}
				}
			}
		}
	}
}

void UMusicSelector::InitCivs(ECivs Player, ECivs Opponent)
{
	PlayerCiv = Player;
	OpponentCiv = Opponent;
}

void UMusicSelector::SetTopWidget(UUpWidget* Widget)
{
	UpWidget = Widget;
}

void UMusicSelector::Skip()
{
	SilenceTime = 0.f;
	Stop();
	UpWidget->SetMusicTitle("");
	UpWidget->SetMusicCivs(TArray<ECivs>());
}