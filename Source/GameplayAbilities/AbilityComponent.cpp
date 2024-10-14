// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "GameFramework/Character.h"
#include "GameplayAbilitiesPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (IsValid(Character))
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (IsValid(PlayerController))
		{
			if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent = InputComponent;
			}

			if (AGameplayAbilitiesPlayerController* ProjectPlayerController = Cast<AGameplayAbilitiesPlayerController>(PlayerController))
			{
				ProjectPlayerController->RegisterAbility(this);
			}
		}
	}
}

void UAbilityComponent::EnterAbility()
{
	OnEnterAbility.Broadcast(this);
}

void UAbilityComponent::LeaveAbility()
{
	OnLeaveAbility.Broadcast(this);
}
