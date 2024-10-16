// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TelekinesisComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYABILITIES_API UTelekinesisComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTelekinesisComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float PickUpRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float DestinationReachThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float ThrowForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float DropAngle;

	/** Telekinesis Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> PickUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	TObjectPtr<UNiagaraSystem> MoveParticleSystem;

	TObjectPtr<UNiagaraComponent> SpawnedVFXComponent;

	bool bIsMovingItem;
	bool bIsHoldingKey;

	void ReleaseItem();

	TObjectPtr<AActor> GetMovedItem() const { return MovedItem; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Called for telekinesis */
	void PickUpItem(const FInputActionValue& Value);
	void ThrowItem(const FInputActionValue& Value);

	void ReleaseKey(const FInputActionValue& Value);

	TObjectPtr<AActor> MovedItem;
	FVector DistanceToItem;

};
