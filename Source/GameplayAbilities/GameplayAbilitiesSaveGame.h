// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayAbilitiesSaveGame.generated.h"

	USTRUCT(BlueprintType)
	struct FLevelData
	{
		GENERATED_BODY()

	public:

		UPROPERTY(BlueprintReadOnly, Category = LevelData)
		bool bIsCompleted;

		UPROPERTY(BlueprintReadOnly, Category = LevelData)
		bool bNoBlink;

		UPROPERTY(BlueprintReadOnly, Category = LevelData)
		bool bNoTelekinesis;

		UPROPERTY(BlueprintReadOnly, Category = LevelData)
		bool bNoPowers;

		UPROPERTY(BlueprintReadOnly, Category = LevelData)
		bool bDevFast;

		UPROPERTY(BlueprintReadOnly, Category = LevelData)
		float BestTime;

		FLevelData() :
			bIsCompleted(false),
			bNoBlink(false),
			bNoTelekinesis(false),
			bNoPowers(false),
			bDevFast(false),
			BestTime(0.0f)
		{ }
	};

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
	TArray<FLevelData> LevelData;

	void InitData(int levelCount);

	UFUNCTION(BlueprintCallable)
	float GetBestTime(int levelIndex);

	void SetBestTime(int levelIndex, float newBestTime);

};
