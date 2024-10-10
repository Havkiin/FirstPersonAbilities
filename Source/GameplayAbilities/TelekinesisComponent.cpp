// Fill out your copyright notice in the Description page of Project Settings.


#include "TelekinesisComponent.h"
#include "GameplayAbilitiesCharacter.h"

// Sets default values for this component's properties
UTelekinesisComponent::UTelekinesisComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsMovingItem = false;
	bIsHoldingKey = false;
}


// Called when the game starts
void UTelekinesisComponent::BeginPlay()
{
	Super::BeginPlay();

	EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &UTelekinesisComponent::PickUpItem);
	EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Completed, this, &UTelekinesisComponent::ReleaseKey);

	EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &UTelekinesisComponent::ThrowItem);

	// Notify the player controller
	EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Completed, this, &UAbilityComponent::LeaveAbility);

}

void UTelekinesisComponent::PickUpItem(const FInputActionValue& Value)
{
	if (bIsHoldingKey && !bIsMovingItem)
		return;

	bIsHoldingKey = true;

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!IsValid(PawnOwner))
		return;

	AGameplayAbilitiesCharacter* ProjectCharacter = Cast<AGameplayAbilitiesCharacter>(PawnOwner);
	if (!IsValid(ProjectCharacter))
		return;

	if (bIsMovingItem)
	{
		FVector NewLocation = PawnOwner->GetActorLocation() + ProjectCharacter->GetPOV_Origin() + PawnOwner->GetControlRotation().Vector() * DistanceToItem.Length();

		MovedItem->SetActorLocation(NewLocation);
		SpawnedComponent->SetWorldLocation(NewLocation);
	}
	else
	{
		FHitResult Hit;

		FVector TraceStart = PawnOwner->GetActorLocation() + ProjectCharacter->GetPOV_Origin();
		FVector TraceEnd = PawnOwner->GetActorLocation() + ProjectCharacter->GetPOV_Origin() + PawnOwner->GetControlRotation().Vector() * PickUpRange;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(PawnOwner);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

		if (Hit.bBlockingHit && Hit.GetActor()->GetRootComponent()->IsSimulatingPhysics())
		{
			MovedItem = Hit.GetActor();
			DistanceToItem = MovedItem->GetActorLocation() - TraceStart;
			bIsMovingItem = true;
			EnterAbility();

			UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(MovedItem->GetRootComponent());
			if (IsValid(Component))
			{
				Component->SetEnableGravity(false);	
				SpawnedComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MoveParticleSystem, MovedItem->GetActorLocation(), MovedItem->GetActorRotation());
			}
		}
	}
}

void UTelekinesisComponent::ThrowItem(const FInputActionValue& Value)
{
	if (bIsMovingItem && IsValid(MovedItem))
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(MovedItem->GetRootComponent());
		AGameplayAbilitiesCharacter* ProjectCharacter = Cast<AGameplayAbilitiesCharacter>(GetOwner());
		if (IsValid(Component) && IsValid(ProjectCharacter))
		{
			FVector Direction = MovedItem->GetActorLocation() - ProjectCharacter->GetActorLocation() - ProjectCharacter->GetPOV_Origin();
			Direction.Normalize();
			FVector ImpulseApplied = Direction * ThrowForce;

			ReleaseItem();
			Component->AddImpulse(ImpulseApplied);
		}
	}
}

void UTelekinesisComponent::ReleaseKey(const FInputActionValue& Value)
{
	bIsHoldingKey = false;
	ReleaseItem();
}

void UTelekinesisComponent::ReleaseItem()
{
	bIsMovingItem = false;

	if (IsValid(MovedItem))
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(MovedItem->GetRootComponent());
		if (IsValid(Component))
		{
			Component->SetEnableGravity(true);
		}

		if (IsValid(SpawnedComponent))
		{
			SpawnedComponent->DestroyInstance();
		}
	}
}
