// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"
#include "EnhancedInputComponent.h"
#include "AreWeBackReloadedCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UBlinkComponent::UBlinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsBlinkValid = false;
}


// Called when the game starts
void UBlinkComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (IsValid(Character))
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (IsValid(PlayerController))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Triggered, this, &UBlinkComponent::PickBlinkLocation);
				EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Completed, this, &UBlinkComponent::Blink);

				SpawnedComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BlinkParticleSystem, FVector::ZeroVector, FRotator::ZeroRotator);
				SpawnedComponent->SetVisibility(false, true);
			}
		}
	}
}

void UBlinkComponent::PickBlinkLocation()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!IsValid(PawnOwner))
		return;

	AAreWeBackReloadedCharacter* ProjectCharacter = Cast<AAreWeBackReloadedCharacter>(PawnOwner);
	if (!IsValid(ProjectCharacter))
		return;

	FHitResult BlockingHit;
	FVector CameraOffset = ProjectCharacter->GetPOV_Origin();

	FVector BlockingTraceStart = PawnOwner->GetActorLocation() + CameraOffset;
	FVector BlockingTraceEnd = PawnOwner->GetActorLocation() + CameraOffset + PawnOwner->GetControlRotation().Vector() * BlinkRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PawnOwner);

	GetWorld()->LineTraceSingleByChannel(BlockingHit, BlockingTraceStart, BlockingTraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

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

		bool bHit = GetWorld()->LineTraceSingleByChannel(BlinkHit, BlinkTraceStart, BlinkTraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

		if (bHit)
		{
			BlinkLocation = BlinkHit.Location + FVector(0.0f, 0.0f, characterHalfHeight);
		}
	}
	// Otherwise, perform a box sweep starting from the actor location, to find blink locations along the way
	else
	{
		FVector BoxHalfSize = FVector(characterRadius, characterRadius, characterHalfHeight);
		bool bHit = GetWorld()->SweepSingleByChannel(BlinkHit, BlockingTraceStart, BlockingTraceEnd, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeBox(BoxHalfSize), QueryParams);

		if (bHit)
		{
			FVector CapsuleOffset = BlockingTraceEnd - BlockingTraceStart;
			CapsuleOffset.Normalize();
			CapsuleOffset *= characterRadius * 2.0f;

			BlinkLocation = BlinkHit.Location + CapsuleOffset;
		}
	}

	if (BlinkHit.bBlockingHit)
	{
		bIsBlinkValid = true;
		SpawnedComponent->SetWorldLocation(BlinkLocation - FVector(0.0f, 0.0f, characterHalfHeight));
	}
	else
	{
		bIsBlinkValid = false;
	}

	SpawnedComponent->SetVisibility(bIsBlinkValid, true);

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
		SpawnedComponent->SetVisibility(false, true);
	}
}
