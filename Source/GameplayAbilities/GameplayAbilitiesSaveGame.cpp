// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitiesSaveGame.h"

void UGameplayAbilitiesSaveGame::SetBestTime(int levelIndex, float newTime)
{
	// No data for this level
	if (levelIndex > BestTimes.Num() - 1)
	{
		BestTimes.SetNum(levelIndex + 1);
		BestTimes[levelIndex] = newTime;
	}
	else if (newTime < BestTimes[levelIndex] || BestTimes[levelIndex] == 0.0f)
	{
		BestTimes[levelIndex] = newTime;
	}
}
