// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbilitiesGameMode.h"
#include "GameplayAbilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilitiesHUD.h"
#include "GameplayAbilitiesPlayerController.h"
#include "GameplayAbilitiesGameInstance.h"
#include "LevelEndZone.h"
#include "GameplayAbilitiesSaveGame.h"

AGameplayAbilitiesGameMode::AGameplayAbilitiesGameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	MainMenuLevel = "MainMenu";
}

void AGameplayAbilitiesGameMode::StartPlay()
{
	Super::StartPlay();

	// Game instance setup
	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (IsValid(gameInstance))
	{
		GameInstance = Cast<UGameplayAbilitiesGameInstance>(gameInstance);
		if (IsValid(GameInstance))
		{
			UGameplayAbilitiesSaveGame* gameSave = GameInstance->GetGameSave();
			gameSave->InitData(Levels.Num());
		}
	}
}

void AGameplayAbilitiesGameMode::LoadMainMenu()
{
	if (!MainMenuLevel.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(MainMenuLevel));
		GameInstance->CurrentLevelIndex = -1;
	}
}

void AGameplayAbilitiesGameMode::LoadLevel(int levelIndex)
{
	if (Levels.IsValidIndex(levelIndex))
	{
		FString LevelToLoad = Levels[levelIndex];
		UGameplayStatics::OpenLevel(GetWorld(), FName(LevelToLoad));
		GameInstance->CurrentLevelIndex = levelIndex;
	}
}
