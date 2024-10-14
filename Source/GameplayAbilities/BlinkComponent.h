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
	UInputAction* BlinkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	UNiagaraSystem* GroundLocationParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	UNiagaraSystem* BlinkLocationParticleSystem;

	UNiagaraComponent* GroundParticleComponent;
	UNiagaraComponent* BlinkParticleComponent;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bIsBlinkValid;
	FVector BlinkLocation;

	void PickBlinkLocation();
	void Blink();

	bool bIsBlinking;
};
