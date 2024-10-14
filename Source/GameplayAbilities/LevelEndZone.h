// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelEndZone.generated.h"

class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelEndRequest);

UCLASS()
class GAMEPLAYABILITIES_API ALevelEndZone : public AActor
{
	GENERATED_BODY()
	
public:

	ALevelEndZone();

	UPROPERTY(BlueprintAssignable, Category = Game)
	FOnLevelEndRequest OnLevelEndRequest;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UCapsuleComponent> TriggerZone;

	UFUNCTION()
	void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
