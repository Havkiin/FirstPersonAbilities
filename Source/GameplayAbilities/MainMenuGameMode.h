// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitiesGameMode.h"
#include "MainMenuGameMode.generated.h"

class UUserWidget;

/**
 * Game Mode while in the main menu
 */
UCLASS()
class GAMEPLAYABILITIES_API AMainMenuGameMode : public AGameplayAbilitiesGameMode
{
	GENERATED_BODY()

	AMainMenuGameMode();

protected:

	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UUserWidget* MainMenuWidget;
	
};
