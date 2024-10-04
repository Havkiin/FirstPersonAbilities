// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameplayAbilitiesPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "AbilityComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilitiesSaveGame.h"

void AGameplayAbilitiesPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		// Add the mapping context so we get controls
		InputSubsystem->AddMappingContext(MovementMappingContext, 0);
	}

	// Game save
	USaveGame* gameSave;

	if (!UGameplayStatics::DoesSaveGameExist("PlayerData", 0))
	{
		gameSave = UGameplayStatics::CreateSaveGameObject(UGameplayAbilitiesSaveGame::StaticClass());
	}
	else
	{
		gameSave = UGameplayStatics::LoadGameFromSlot("PlayerData", 0);
	}

	UGameplayAbilitiesSaveGame* projectGameSave = Cast<UGameplayAbilitiesSaveGame>(gameSave);
	if (IsValid(projectGameSave))
	{
		GameSave = projectGameSave;
	}
}

void AGameplayAbilitiesPlayerController::RegisterAbility(UAbilityComponent* Ability)
{
	UInputMappingContext* MappingContext = Ability->GetMappingContext();
	AbilityMappingContexts.Add(MappingContext);
	InputSubsystem->AddMappingContext(MappingContext, 0);

	// Bind delegate to remove controls for other abilities while an ability is in use
	Ability->OnEnterAbility.BindUObject(this, &AGameplayAbilitiesPlayerController::RemoveOtherMappingContexts);
	Ability->OnLeaveAbility.BindUObject(this, &AGameplayAbilitiesPlayerController::AddOtherMappingContexts);
}

void AGameplayAbilitiesPlayerController::SetInputToUIOnly(UUserWidget* FocusWidget)
{
	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(FocusWidget->TakeWidget());
	SetInputMode(inputMode);
	bShowMouseCursor = true;
}

void AGameplayAbilitiesPlayerController::SetInputToGameOnly()
{
	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
	bShowMouseCursor = false;
}

float AGameplayAbilitiesPlayerController::GetBestTime(int levelIndex)
{
	float bestTime = 0.0f;

	if (IsValid(GameSave))
	{
		bestTime = GameSave->BestTimes[levelIndex];
	}

	return bestTime;
}

void AGameplayAbilitiesPlayerController::SetBestTime(int levelIndex, float newTime)
{
	if (IsValid(GameSave))
	{
		GameSave->SetBestTime(levelIndex, newTime);
		UGameplayStatics::SaveGameToSlot(GameSave, "PlayerData", 0);
	}
}

void AGameplayAbilitiesPlayerController::AddOtherMappingContexts(UAbilityComponent* Ability)
{
	for (UInputMappingContext* mappingContext : AbilityMappingContexts)
	{
		if (mappingContext != Ability->GetMappingContext() && IsValid(InputSubsystem))
		{
			InputSubsystem->AddMappingContext(mappingContext, 0);
		}
	}
}

void AGameplayAbilitiesPlayerController::RemoveOtherMappingContexts(UAbilityComponent* Ability)
{
	for (UInputMappingContext* mappingContext : AbilityMappingContexts)
	{
		if (mappingContext != Ability->GetMappingContext() && IsValid(InputSubsystem))
		{
			InputSubsystem->RemoveMappingContext(mappingContext);
		}
	}
}
