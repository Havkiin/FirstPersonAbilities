// Copyright Epic Games, Inc. All Rights Reserved.


#include "GameplayAbilitiesPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "AbilityComponent.h"
#include "InputMappingContext.h"

void AGameplayAbilitiesPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		// Add the mapping context so we get controls
		InputSubsystem->AddMappingContext(MovementMappingContext, 0);
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
