// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayAbilitiesGameInstance.generated.h"

class UGameplayAbilitiesSaveGame;

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITIES_API UGameplayAbilitiesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	int CurrentLevelIndex;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	UGameplayAbilitiesSaveGame* GetGameSave() const { return GameSave; }

protected:

	UGameplayAbilitiesSaveGame* GameSave;
	
};
