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

	bIsLevelEnded = false;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
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

	// Player controller setup
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(playerController))
	{
		PlayerController = Cast<AGameplayAbilitiesPlayerController>(playerController);

		// Character setup
		APawn* playerPawn = PlayerController->GetPawn();
		if (IsValid(playerPawn))
		{
			AGameplayAbilitiesCharacter* playerCharacter = Cast<AGameplayAbilitiesCharacter>(playerPawn);
			if (IsValid(playerCharacter))
			{
				playerCharacter->OnCharacterDeath.AddUniqueDynamic(this, &AGameplayAbilitiesGameMode::RestartLevel);
			}
		}
	}

	// Game instance setup
	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (IsValid(gameInstance))
	{
		GameInstance = Cast<UGameplayAbilitiesGameInstance>(gameInstance);
		GameInstance->CurrentLevelIndex = CurrentLevel;
	}

	// Level end zone setup
	AActor* endZoneActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelEndZone::StaticClass());
	if (IsValid(endZoneActor))
	{
		ALevelEndZone* endZone = Cast<ALevelEndZone>(endZoneActor);
		if (IsValid(endZone))
		{
			endZone->OnLevelEndRequest.AddUniqueDynamic(this, &AGameplayAbilitiesGameMode::EndLevel);
		}
	}
}

void AGameplayAbilitiesGameMode::EndLevel()
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

void AGameplayAbilitiesGameMode::RestartLevel()
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
