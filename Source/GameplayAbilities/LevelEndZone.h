// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelEndZone.generated.h"

class UCapsuleComponent;

UCLASS()
class GAMEPLAYABILITIES_API ALevelEndZone : public AActor
{
	GENERATED_BODY()
	
public:

	ALevelEndZone();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UCapsuleComponent* TriggerZone;

	UFUNCTION()
	void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
