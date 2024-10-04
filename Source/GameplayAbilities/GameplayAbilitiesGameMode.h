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

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Levels;

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

	UFUNCTION(BlueprintCallable)
	void RestartCurrentLevel();

	virtual void StartPlay() override;

	// Ends the current level and displays end score
	void EndCurrentLevel();

protected:

	virtual void Tick(float DeltaTime) override;

	AGameplayAbilitiesPlayerController* PlayerController;
	UGameplayAbilitiesGameInstance* GameInstance;

	float LevelTime;
	bool bIsLevelEnded;

};



