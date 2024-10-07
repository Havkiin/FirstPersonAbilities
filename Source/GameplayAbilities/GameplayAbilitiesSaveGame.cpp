// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitiesSaveGame.h"

FCriticalSection CriticalSection;

void UGameplayAbilitiesSaveGame::InitData(int levelCount)
{
	FScopeLock Lock(&CriticalSection);

	if (LevelData.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("levelCount: %d"), levelCount);

		FLevelData newLevelData = FLevelData();
		LevelData.Init(newLevelData, levelCount);
	}
}

void UGameplayAbilitiesSaveGame::SetBestTime(int levelIndex, float newTime)
{
	// Prevents Save Game object to be saved while we assign the data
	FScopeLock Lock(&CriticalSection);

	if (!LevelData.IsValidIndex(levelIndex))
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("New time: %f"), newTime);
	UE_LOG(LogTemp, Log, TEXT("Best time: %f"), LevelData[levelIndex].BestTime);

	if (newTime < LevelData[levelIndex].BestTime || LevelData[levelIndex].BestTime == 0.0f)
	{
		LevelData[levelIndex].BestTime = newTime;
	}
}

float UGameplayAbilitiesSaveGame::GetBestTime(int levelIndex)
{
	return LevelData[levelIndex].BestTime;
}
