// Fill out your copyright notice in the Description page of Project Settings.

#include "Sinah.h"
#include "MusicItem.h"

UMusicItem::UMusicItem() : Turns(1) {}
void UMusicItem::Init(int Victory, int Epicness, TArray<ECivs> Civ, USoundWave* Sound, FString Info)
{
	VictoryPoints = Victory;
	EpicnessPoints = Epicness;
	Civs = Civ;
	Music = Sound;
	Title = Info;
}

int UMusicItem::GetPercents(int Victory, int Epicness, ECivs Player, ECivs Opponent)
{
	float CivFactor = 2.f;

	if (Civs.Find(Player) != INDEX_NONE)
		CivFactor = 1.f;
	else if (Civs.Find(Opponent) != INDEX_NONE)
		CivFactor = 1.5f;

	return (int) ((100 - (int) (sqrt((abs(VictoryPoints - Victory) * abs(VictoryPoints - Victory) + abs(EpicnessPoints - Epicness) * abs(EpicnessPoints - Epicness)) / 2))) / Turns / CivFactor);
}

void UMusicItem::TurnPass()
{
	if (Turns > 1)
		Turns--;
}

USoundWave* UMusicItem::Play()
{
	Turns = 4;

	return Music;
}

FString UMusicItem::GetTitle()
{
	return Title;
}
TArray<ECivs> UMusicItem::GetCivs()
{
	return Civs;
}