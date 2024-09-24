// Copyright Epic Games, Inc. All Rights Reserved.

#include "AreWeBackReloadedGameMode.h"
#include "AreWeBackReloadedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAreWeBackReloadedGameMode::AAreWeBackReloadedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
