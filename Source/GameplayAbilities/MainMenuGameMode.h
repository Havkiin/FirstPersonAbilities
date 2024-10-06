// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITIES_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AMainMenuGameMode();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UUserWidget* MainMenuWidget;
	
};
