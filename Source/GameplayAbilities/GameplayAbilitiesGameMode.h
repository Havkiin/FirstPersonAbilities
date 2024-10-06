// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayAbilitiesGameMode.generated.h"

class AGameplayAbilitiesPlayerController;
class UGameplayAbilitiesGameInstance;

UCLASS(minimalapi)
class AGameplayAbilitiesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGameplayAbilitiesGameMode();

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> Levels;

	// Temp, for test purposes only
	UPROPERTY(EditDefaultsOnly)
	int CurrentLevel;

	UFUNCTION(BlueprintCallable)
	float GetLevelTime() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsLevelEnded() const;

	UFUNCTION(BlueprintCallable)
	AGameplayAbilitiesPlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintCallable)
	UGameplayAbilitiesGameInstance* GetGameInstance() const;

	UFUNCTION (BlueprintCallable)
	void LoadLevel(int levelIndex);

	// Ends the current level and displays end score
	UFUNCTION(BlueprintCallable)
	void EndLevel();

	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	virtual void StartPlay() override;

protected:

	virtual void Tick(float DeltaTime) override;

	AGameplayAbilitiesPlayerController* PlayerController;
	UGameplayAbilitiesGameInstance* GameInstance;

	float LevelTime;
	bool bIsLevelEnded;

};



