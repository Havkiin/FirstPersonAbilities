// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbilitiesGameMode.h"
#include "GameplayAbilitiesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameplayAbilitiesGameMode::AGameplayAbilitiesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
