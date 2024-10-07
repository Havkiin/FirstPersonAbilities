// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitiesGameInstance.h"
#include "GameplayAbilitiesSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UGameplayAbilitiesGameInstance::Init()
{
	Super::Init();
	
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

	// Beware the angry Garbage Collector
	gameSave->AddToRoot();

	UGameplayAbilitiesSaveGame* projectGameSave = Cast<UGameplayAbilitiesSaveGame>(gameSave);
	if (IsValid(projectGameSave))
	{
		GameSave = projectGameSave;
	}
}

void UGameplayAbilitiesGameInstance::InitSaveData(int levelCount)
{
	if (IsValid(GameSave))
	{
		GameSave->InitData(levelCount);
	}
}
