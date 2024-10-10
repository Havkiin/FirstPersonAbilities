// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitiesSaveGame.h"

FCriticalSection CriticalSection;

void UGameplayAbilitiesSaveGame::InitData(int levelCount)
{
	FScopeLock Lock(&CriticalSection);

	if (LevelData.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("levelCount: %d"), levelCount);

		FLevelPlayerData newLevelData = FLevelPlayerData();
		LevelData.Init(newLevelData, levelCount);
	}
}

void UGameplayAbilitiesSaveGame::SetLevelData(int levelIndex, float newTime, bool noBlink, bool noTelekinesis, bool devFast)
{
	// Prevents Save Game object to be saved while we assign the data
	FScopeLock Lock(&CriticalSection);

	if (!LevelData.IsValidIndex(levelIndex))
	{
		return;
	}

	FLevelPlayerData levelData = LevelData[levelIndex];

	levelData.bIsCompleted = true;

	if (newTime < levelData.BestTime || levelData.BestTime == 0.0f)
	{
		levelData.BestTime = newTime;
	}

	if (!levelData.bNoBlink && noBlink)
	{
		levelData.bNoBlink = true;
	}

	if (!levelData.bNoTelekinesis && noTelekinesis)
	{
		levelData.bNoTelekinesis = true;
	}

	if (!levelData.bNoPowers && noBlink && noTelekinesis)
	{
		levelData.bNoPowers = true;
	}

	if (!levelData.bDevFast && devFast)
	{
		levelData.bDevFast = true;
	}

	LevelData[levelIndex] = levelData;
}

int UGameplayAbilitiesSaveGame::GetLastCompletedLevel() const
{
	int index = 0;

	while (index < LevelData.Num() - 1 && LevelData[index].bIsCompleted)
	{
		index++;
	}

	return index;
}
