// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilitiesHUD.h"
#include "Blueprint/UserWidget.h"

void AGameplayAbilitiesHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget->AddToViewport();
	}
}

void AGameplayAbilitiesHUD::DisplayEndScore()
{
	PlayerHUDWidget->RemoveFromViewport();
	EndScoreWidget->AddToViewport();
}
