// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEndZone.h"
#include "GameFramework/GameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "GameplayAbilitiesCharacter.h"
#include "GameplayAbilitiesGameMode.h"

ALevelEndZone::ALevelEndZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerZone = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Zone"));
}

void ALevelEndZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ALevelEndZone::HandleOverlap);
}

void ALevelEndZone::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AGameplayAbilitiesCharacter>(OtherActor))
	{
		OnLevelEndRequest.Broadcast();
	}
}
