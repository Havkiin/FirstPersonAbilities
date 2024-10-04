// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayAbilitiesPlayerController.generated.h"

class UInputMappingContext;
class UAbilityComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UUserWidget;
class UGameplayAbilitiesSaveGame;

/**
 *
 */
UCLASS()
class GAMEPLAYABILITIES_API AGameplayAbilitiesPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	float GetBestTime(int levelIndex);

	void SetBestTime(int levelIndex, float time);

	void SetInputToUIOnly(UUserWidget* FocusWidget);
	void SetInputToGameOnly();

	void RegisterAbility(UAbilityComponent* Ability);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* MovementMappingContext;

	UGameplayAbilitiesSaveGame* GameSave;

	/***** ABILITIES *****/
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem;
	TArray<UInputMappingContext*> AbilityMappingContexts;

	void AddOtherMappingContexts(UAbilityComponent* Ability);
	void RemoveOtherMappingContexts(UAbilityComponent* Ability);

};
