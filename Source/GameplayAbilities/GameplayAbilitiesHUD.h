// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameplayAbilitiesHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITIES_API AGameplayAbilitiesHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Widgets)
	UUserWidget* PlayerHUDWidget;

	UPROPERTY(EditAnywhere, Category = Widgets)
	UUserWidget* EndScoreWidget;

	UFUNCTION(BlueprintCallable)
	void DisplayEndScore();

protected:

	virtual void BeginPlay() override;

};
