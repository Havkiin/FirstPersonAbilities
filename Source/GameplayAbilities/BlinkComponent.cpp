// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitiesCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	//EnhancedInputComponent->BindAction(BlinkAction, ETriggerEvent::Completed, this, &UAbilityComponent::LeaveAbility);

	if (IsValid(GroundLocationParticleSystem))
	{
		GroundParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GroundLocationParticleSystem, FVector::ZeroVector, FRotator::ZeroRotator);
		GroundParticleComponent->SetVisibility(false, true);
	}

	if (IsValid(BlinkLocationParticleSystem))
	{
		BlinkParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BlinkLocationParticleSystem, FVector::ZeroVector, FRotator::ZeroRotator);
		BlinkParticleComponent->SetVisibility(false, true);
	}

}

void UBlinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBlinking)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (IsValid(Character))
		{
			float distanceToDestination = FVector::Distance(Character->GetActorLocation(), BlinkLocation);
			if (distanceToDestination < ArriveThreshold)
			{
				Character->GetCharacterMovement()->StopMovementImmediately();
				Character->SetActorLocation(BlinkLocation, false, nullptr, ETeleportType::ResetPhysics);

				bIsBlinking = false;
				Character->bSimGravityDisabled = false;
				Character->SetActorEnableCollision(true);
				LeaveAbility();
			}
		}
	}
}

void UBlinkComponent::PickBlinkLocation()
{
	if (bIsBlinking)
		return;

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

	FVector GroundVFXLocation = FVector::ZeroVector;
	FVector BlinkVFXLocation = FVector::ZeroVector;

	// If we hit an actor, perform a raycast downwards
	if (BlockingHit.GetActor())
	{
		bIsBlinkValid = true;

		BlinkTraceStart = BlockingHit.Location;

		FVector CapsuleOffset = BlinkTraceStart - BlockingTraceStart;
		CapsuleOffset.Normalize();
		CapsuleOffset *= characterRadius;

		BlinkTraceStart -= CapsuleOffset;

		FVector BlinkTraceEnd = BlinkTraceStart + FVector::DownVector * 350.0f;

		bool bHitDown = World->LineTraceSingleByChannel(BlinkHit, BlinkTraceStart, BlinkTraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

		BlinkLocation = BlinkTraceStart;
		BlinkVFXLocation = BlinkTraceStart;

		// We hit a wall or the ground
		if (bHitDown)
		{
			// We hit the ground or a lower section of a wall (lower than our character's half height)
			float groundDistance = FVector::Distance(BlinkHit.Location, BlinkTraceStart);
			if (groundDistance < characterHalfHeight)
			{
				BlinkLocation = BlinkHit.Location + FVector(0.0f, 0.0f, characterHalfHeight);
				BlinkVFXLocation = BlinkHit.Location;
			}

			GroundVFXLocation = BlinkHit.Location;
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

			// We are going over a ledge
			if (bHitDown)
			{
				bIsBlinkValid = true;
				BlinkLocation = BlinkHit.Location + FVector(0.0f, 0.0f, characterHalfHeight);
				GroundVFXLocation = BlinkHit.Location;
				BlinkVFXLocation = BlinkHit.Location;
			}
		}
	}

	if (bIsBlinkValid)
	{
		GroundParticleComponent->SetWorldLocation(GroundVFXLocation);
		BlinkParticleComponent->SetWorldLocation(BlinkVFXLocation);
	}

	GroundParticleComponent->SetVisibility(bIsBlinkValid, true);
	BlinkParticleComponent->SetVisibility(bIsBlinkValid, true);
}

void UBlinkComponent::Blink()
{
	if (!bIsBlinkValid)
		return;

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (IsValid(PawnOwner))
	{
		//PawnOwner->SetActorLocation(BlinkLocation, false, nullptr, ETeleportType::ResetPhysics);

		ACharacter* Character = Cast<ACharacter>(PawnOwner);
		if (IsValid(Character))
		{
			FVector direction = BlinkLocation - Character->GetActorLocation();
			direction.Normalize();
			Character->LaunchCharacter(direction * LaunchForce, true, true);
			Character->bSimGravityDisabled = true;
			Character->SetActorEnableCollision(false);

			bIsBlinkValid = false;
			bIsBlinking = true;
			GroundParticleComponent->SetVisibility(false, true);
			BlinkParticleComponent->SetVisibility(false, true);
		}
	}
}
