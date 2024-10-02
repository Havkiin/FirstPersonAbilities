// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitiesCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UBlinkComponent::UBlinkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsBlinkValid = false;
}


// Called when the game starts
void UBlinkComponent::BeginPlay()
{
	Super::BeginPlay();

	EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Triggered, this, &UBlinkComponent::PickBlinkLocation);
	EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Completed, this, &UBlinkComponent::Blink);

	// Notify the player controller
	EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Started, this, &UAbilityComponent::EnterAbility);
	EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Completed, this, &UAbilityComponent::LeaveAbility);

	SpawnedParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BlinkParticleSystem, FVector::ZeroVector, FRotator::ZeroRotator);
	SpawnedParticleComponent->SetVisibility(false, true);

}

void UBlinkComponent::PickBlinkLocation()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!IsValid(PawnOwner))
		return;

	AGameplayAbilitiesCharacter* ProjectCharacter = Cast<AGameplayAbilitiesCharacter>(PawnOwner);
	if (!IsValid(ProjectCharacter))
		return;

	FHitResult BlockingHit;
	FVector CameraOffset = ProjectCharacter->GetPOV_Origin();

	FVector BlockingTraceStart = PawnOwner->GetActorLocation() + CameraOffset;
	FVector BlockingTraceEnd = PawnOwner->GetActorLocation() + CameraOffset + PawnOwner->GetControlRotation().Vector() * BlinkRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PawnOwner);
	UWorld* World = GetWorld();

	World->LineTraceSingleByChannel(BlockingHit, BlockingTraceStart, BlockingTraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

	FHitResult BlinkHit;
	FVector BlinkTraceStart = BlockingTraceEnd;

	float characterRadius = ProjectCharacter->GetCapsuleRadius();
	float characterHalfHeight = ProjectCharacter->GetCapsuleHalfHeight();

	// If we hit an actor, perform a raycast downwards
	if (BlockingHit.GetActor())
	{
		BlinkTraceStart = BlockingHit.Location;

		FVector CapsuleOffset = BlinkTraceStart - BlockingTraceStart;
		CapsuleOffset.Normalize();
		CapsuleOffset *= characterRadius;

		BlinkTraceStart -= CapsuleOffset;

		FVector BlinkTraceEnd = BlinkTraceStart + FVector::DownVector * 350.0f;

		bool bHitDown = World->LineTraceSingleByChannel(BlinkHit, BlinkTraceStart, BlinkTraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

		if (bHitDown)
		{
			BlinkLocation = BlinkHit.Location + FVector(0.0f, 0.0f, characterHalfHeight);
		}
	}
	// Otherwise, perform a box sweep starting from the actor location, to find blink locations along the way
	else
	{
		FVector BoxHalfSize = FVector(characterRadius, characterRadius, characterHalfHeight);
		bool bHit = World->SweepSingleByChannel(BlinkHit, BlockingTraceStart, BlockingTraceEnd, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeBox(BoxHalfSize), QueryParams);

		if (bHit)
		{
			FVector CapsuleOffset = BlockingTraceEnd - BlockingTraceStart;
			CapsuleOffset.Normalize();
			CapsuleOffset *= characterRadius * 2.0f;
			CapsuleOffset += FVector::UpVector * characterRadius * 2.0f;

			BlinkTraceStart = BlinkHit.Location + CapsuleOffset;
			FVector BlinkTraceEnd = BlinkTraceStart + FVector::DownVector * 350.0f;

			bool bHitDown = World->LineTraceSingleByChannel(BlinkHit, BlinkTraceStart, BlinkTraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

			if (bHitDown)
			{
				BlinkLocation = BlinkHit.Location + FVector(0.0f, 0.0f, characterHalfHeight);
			}
		}
	}

	if (BlinkHit.bBlockingHit)
	{
		bIsBlinkValid = true;
		SpawnedParticleComponent->SetWorldLocation(BlinkLocation - FVector(0.0f, 0.0f, characterHalfHeight));
	}
	else
	{
		bIsBlinkValid = false;
	}

	SpawnedParticleComponent->SetVisibility(bIsBlinkValid, true);

}

void UBlinkComponent::Blink()
{
	if (!bIsBlinkValid)
		return;

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (IsValid(PawnOwner))
	{
		PawnOwner->SetActorLocation(BlinkLocation);
		bIsBlinkValid = false;
		SpawnedParticleComponent->SetVisibility(false, true);
	}
}
