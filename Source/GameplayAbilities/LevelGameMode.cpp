// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGameMode.h"

#include "GameplayAbilitiesGameMode.h"
#include "GameplayAbilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilitiesHUD.h"
#include "GameplayAbilitiesPlayerController.h"
#include "GameplayAbilitiesGameInstance.h"
#include "LevelEndZone.h"
#include "GameplayAbilitiesSaveGame.h"

ALevelGameMode::ALevelGameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsLevelEnded = false;
}

void ALevelGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsLevelEnded)
	{
		LevelTime += DeltaTime;
	}
}

void ALevelGameMode::StartPlay()
{
	Super::StartPlay();

	LevelTime = 0.0f;

	// Player controller setup
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(playerController))
	{
		PlayerController = Cast<AGameplayAbilitiesPlayerController>(playerController);
		PlayerController->SetInputToGameOnly();

		// Character setup
		APawn* playerPawn = PlayerController->GetPawn();
		if (IsValid(playerPawn))
		{
			AGameplayAbilitiesCharacter* playerCharacter = Cast<AGameplayAbilitiesCharacter>(playerPawn);
			if (IsValid(playerCharacter))
			{
				playerCharacter->OnCharacterDeath.AddUniqueDynamic(this, &ALevelGameMode::RestartLevel);
			}
		}
	}

	// Level end zone setup
	AActor* endZoneActor = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelEndZone::StaticClass());
	if (IsValid(endZoneActor))
	{
		ALevelEndZone* endZone = Cast<ALevelEndZone>(endZoneActor);
		if (IsValid(endZone))
		{
			endZone->OnLevelEndRequest.AddUniqueDynamic(this, &ALevelGameMode::EndLevel);
		}
	}
}

void ALevelGameMode::EndLevel()
{
	bIsLevelEnded = true;

	if (IsValid(PlayerController))
	{
		AGameplayAbilitiesHUD* gameHUD = Cast<AGameplayAbilitiesHUD>(PlayerController->GetHUD());
		if (IsValid(gameHUD))
		{
			int levelIndex = GameInstance->CurrentLevelIndex;
			UGameplayAbilitiesSaveGame* gameSave = GameInstance->GetGameSave();

			UE_LOG(LogTemp, Log, TEXT("levelIndex: %d"), levelIndex);
			gameSave->SetBestTime(levelIndex, LevelTime);
			UGameplayStatics::SaveGameToSlot(gameSave, "PlayerData", 0);

			gameHUD->DisplayEndScore();
			PlayerController->SetInputToUIOnly(gameHUD->EndScoreWidget);
			PlayerController->FlushPressedKeys();
		}
	}
}

void ALevelGameMode::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	if (IsValid(PlayerController))
	{
		PlayerController->SetInputToGameOnly();
	}
}
