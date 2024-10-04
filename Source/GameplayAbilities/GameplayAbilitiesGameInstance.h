// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayAbilitiesGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITIES_API UGameplayAbilitiesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UGameplayAbilitiesGameInstance();

	UPROPERTY(BlueprintReadOnly)
	int CurrentLevelIndex;
	
};
