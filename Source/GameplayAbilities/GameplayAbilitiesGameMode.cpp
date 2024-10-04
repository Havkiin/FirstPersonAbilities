// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbilitiesGameMode.h"
#include "GameplayAbilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilitiesHUD.h"
#include "GameplayAbilitiesPlayerController.h"
#include "GameplayAbilitiesGameInstance.h"

AGameplayAbilitiesGameMode::AGameplayAbilitiesGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsLevelEnded = false;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	Levels = { TEXT("Sandbox"), TEXT("Level1") };

}

void AGameplayAbilitiesGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsLevelEnded)
	{
		LevelTime += DeltaTime;
	}
}

void AGameplayAbilitiesGameMode::StartPlay()
{
	Super::StartPlay();

	LevelTime = 0.0f;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(playerController))
	{
		PlayerController = Cast<AGameplayAbilitiesPlayerController>(playerController);
	}

	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (IsValid(gameInstance))
	{
		GameInstance = Cast<UGameplayAbilitiesGameInstance>(gameInstance);
	}
}

void AGameplayAbilitiesGameMode::EndCurrentLevel()
{
	bIsLevelEnded = true;

	if (IsValid(PlayerController))
	{
		AGameplayAbilitiesHUD* gameHUD = Cast<AGameplayAbilitiesHUD>(PlayerController->GetHUD());
		if (IsValid(gameHUD))
		{
			int levelIndex = GameInstance->CurrentLevelIndex;
			PlayerController->SetBestTime(levelIndex, LevelTime);

			gameHUD->DisplayEndScore();
			PlayerController->SetInputToUIOnly(gameHUD->EndScoreWidget);
			PlayerController->FlushPressedKeys();
		}
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

void AGameplayAbilitiesGameMode::RestartCurrentLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	if (IsValid(PlayerController))
	{
		PlayerController->SetInputToGameOnly();
	}
}

float AGameplayAbilitiesGameMode::GetLevelTime() const
{
	return LevelTime;
}

bool AGameplayAbilitiesGameMode::GetIsLevelEnded() const
{
	return bIsLevelEnded;
}

AGameplayAbilitiesPlayerController* AGameplayAbilitiesGameMode::GetPlayerController() const
{
	return PlayerController;
}

UGameplayAbilitiesGameInstance* AGameplayAbilitiesGameMode::GetGameInstance() const
{
	return GameInstance;
}
