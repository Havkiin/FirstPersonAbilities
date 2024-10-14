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

	UPROPERTY(EditDefaultsOnly, Category = Levels)
	FString MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Levels)
	TArray<FString> Levels;

	UFUNCTION(BlueprintCallable)
	UGameplayAbilitiesGameInstance* GetGameInstance() const { return GameInstance; }

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION (BlueprintCallable)
	void LoadLevel(int levelIndex);

	virtual void StartPlay() override;

protected:

	TObjectPtr<UGameplayAbilitiesGameInstance> GameInstance;

};