// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayAbilitiesPlayerController.h"

AMainMenuGameMode::AMainMenuGameMode() : Super()
{
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
	if (IsValid(MainMenuWidgetClass))
	{
		// Create main menu widget
		UUserWidget* mainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (IsValid(mainMenuWidget))
		{
			mainMenuWidget->AddToViewport();

			APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (IsValid(playerController))
			{
				AGameplayAbilitiesPlayerController* projectPlayerController = Cast<AGameplayAbilitiesPlayerController>(playerController);
				if (IsValid(projectPlayerController))
				{
					// Set input mode to UI
					projectPlayerController->SetInputToUIOnly(mainMenuWidget);
				}
			}
		}
	}
}
