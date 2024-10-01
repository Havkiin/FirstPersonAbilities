// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlinkComponent.generated.h"

class UInputAction;
class UNiagaraSystem;
class UNiagaraComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYABILITIES_API UBlinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlinkComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	float BlinkRange;

	/** Telekinesis Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BlinkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	UNiagaraSystem* BlinkParticleSystem;

	UNiagaraComponent* SpawnedComponent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsBlinkValid;
	FVector BlinkLocation;

	void PickBlinkLocation();
	void Blink();
};
