// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitiesGameMode.h"
#include "LevelGameMode.generated.h"

class AGameplayAbilitiesPlayerController;
class UGameplayAbilitiesGameInstance;
class UAbilityComponent;

/**
 * Game Mode while in a level (in-game)
 */
UCLASS()
class GAMEPLAYABILITIES_API ALevelGameMode : public AGameplayAbilitiesGameMode
{
	GENERATED_BODY()

public:

	ALevelGameMode();

	UFUNCTION(BlueprintCallable)
	float GetLevelTime() const { return LevelTime; }

	UFUNCTION(BlueprintCallable)
	bool GetIsLevelEnded() const { return bIsLevelEnded; }

	UFUNCTION(BlueprintCallable)
	AGameplayAbilitiesPlayerController* GetPlayerController() const { return PlayerController; }

	// Ends the current level and displays end score
	UFUNCTION(BlueprintCallable)
	void EndLevel();

	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	virtual void StartPlay() override;

protected:

	virtual void Tick(float DeltaTime) override;

	AGameplayAbilitiesPlayerController* PlayerController;

	float LevelTime;
	bool bIsLevelEnded;
	bool bNoBlink;
	bool bNoTelekinesis;

	void UpdateAbilityUseData(UAbilityComponent* Ability);
	
};
