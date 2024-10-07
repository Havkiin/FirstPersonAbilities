// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbilitiesGameMode.h"
#include "GameplayAbilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilitiesHUD.h"
#include "GameplayAbilitiesPlayerController.h"
#include "GameplayAbilitiesGameInstance.h"
#include "LevelEndZone.h"

AGameplayAbilitiesGameMode::AGameplayAbilitiesGameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameplayAbilitiesGameMode::StartPlay()
{
	Super::StartPlay();

	// Game instance setup
	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (IsValid(gameInstance))
	{
		GameInstance = Cast<UGameplayAbilitiesGameInstance>(gameInstance);
		GameInstance->InitSaveData(Levels.Num());
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

UGameplayAbilitiesGameInstance* AGameplayAbilitiesGameMode::GetGameInstance() const
{
	return GameInstance;
}
