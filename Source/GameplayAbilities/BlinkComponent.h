// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "BlinkComponent.generated.h"

class UInputAction;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEPLAYABILITIES_API UBlinkComponent : public UAbilityComponent
{
	GENERATED_BODY()

public:	
	UBlinkComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float BlinkRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float LaunchForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float ArriveThreshold;

	/** Telekinesis Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> BlinkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	TObjectPtr<UNiagaraSystem> GroundLocationParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	TObjectPtr<UNiagaraSystem> BlinkLocationParticleSystem;

	UFUNCTION(BlueprintCallable)
	void ResetBlinkCount();

	TObjectPtr<UNiagaraComponent> GroundParticleComponent;
	TObjectPtr<UNiagaraComponent> BlinkParticleComponent;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PickBlinkLocation();
	void Blink();

	bool bIsBlinkValid;
	bool bIsBlinking;
	int BlinkCount;
	FVector BlinkLocation;
};
