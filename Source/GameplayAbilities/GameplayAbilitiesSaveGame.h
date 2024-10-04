// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayAbilitiesSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITIES_API UGameplayAbilitiesSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	// Level index, best time
	UPROPERTY(BlueprintReadOnly)
	TArray<float> BestTimes;

	void SetBestTime(int levelIndex, float newBestTime);

};
