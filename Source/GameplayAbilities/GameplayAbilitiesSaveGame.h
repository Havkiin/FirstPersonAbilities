// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayAbilitiesSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLevelPlayerData
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

	FLevelPlayerData() :
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
	TArray<FLevelPlayerData> LevelData;

	UFUNCTION(BlueprintCallable)
	int GetLastCompletedLevel() const;

	void InitData(int levelCount);
	void SetLevelData(int levelIndex, float newTime, bool noBlink, bool noTelekinesis, bool devFast);

};
