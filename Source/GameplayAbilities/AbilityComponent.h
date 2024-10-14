// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UEnhancedInputComponent;
class UInputMappingContext;

DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityStateChangeDelegate, UAbilityComponent*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYABILITIES_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	FAbilityStateChangeDelegate OnEnterAbility;
	FAbilityStateChangeDelegate OnLeaveAbility;

	virtual void EnterAbility();
	virtual void LeaveAbility();

	TObjectPtr<UInputMappingContext> GetMappingContext() const { return MappingContext; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> MappingContext;

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

};
