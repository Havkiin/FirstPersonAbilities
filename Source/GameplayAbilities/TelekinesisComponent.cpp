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

	if (IsValid(MoveParticleSystem))
	{
		SpawnedVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MoveParticleSystem, FVector::ZeroVector, FRotator::ZeroRotator);
		SpawnedVFXComponent->SetVisibility(false, true);
	}

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

	FHitResult Hit;

	FVector TraceStart = PawnOwner->GetActorLocation() + ProjectCharacter->GetPOV_Origin();
	FVector TraceEnd = PawnOwner->GetActorLocation() + ProjectCharacter->GetPOV_Origin() + PawnOwner->GetControlRotation().Vector() * PickUpRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PawnOwner);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, QueryParams);

	// We are moving an item
	if (bIsMovingItem)
	{
		FVector NewLocation = PawnOwner->GetActorLocation() + ProjectCharacter->GetPOV_Origin() + PawnOwner->GetControlRotation().Vector() * DistanceToItem.Length();
		FVector MovedItemLocation = MovedItem->GetActorLocation();

		UPrimitiveComponent* MovedItemComponent = Cast<UPrimitiveComponent>(MovedItem->GetRootComponent());
		FVector MoveDirection = NewLocation - MovedItemLocation;
		MoveDirection.Normalize();

		// Smooth out movement
		float distanceToDestination = FVector::Distance(MovedItemLocation, NewLocation);
		float speedModifier = FMath::Clamp(distanceToDestination / DestinationReachThreshold, 0.0f, 1.0f);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(MovedItem);

		// Stop if we would move through a wall
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MovedItemLocation, NewLocation, ECC_Visibility, CollisionParams);
		if (bHit && Hit.GetActor())
		{
			speedModifier = 0.0f;
		}

		// Stop if we are close enough to the aimed direction or we would go through an object
		if (distanceToDestination < 5.0f)
		{
			speedModifier = 0.0f;
		}

		// Move the item and its VFX
		MoveDirection *= (MoveSpeed * speedModifier);

		MovedItemComponent->SetPhysicsLinearVelocity(MoveDirection);
		MovedItemComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

		FVector newItemLocation = MovedItem->GetActorLocation();
		SpawnedVFXComponent->SetWorldLocation(newItemLocation);

		// If the moved item got stuck against something or we lost line of sight, release it
		FVector toItem = newItemLocation - TraceStart;
		FVector toAim = TraceEnd - TraceStart;
		toItem.Normalize();
		toAim.Normalize();

		float dot = toItem.Dot(toAim);
		float angleToItem = FMath::Acos(dot);
		angleToItem = FMath::RadiansToDegrees(angleToItem);

		if (angleToItem > DropAngle)
		{
			ReleaseItem();
		}
	}
	// We are looking for an item to move
	else if (Hit.bBlockingHit && Hit.GetActor()->GetRootComponent()->IsSimulatingPhysics())
	{
		MovedItem = Hit.GetActor();
		DistanceToItem = MovedItem->GetActorLocation() - TraceStart;
		bIsMovingItem = true;
		EnterAbility();

		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(MovedItem->GetRootComponent());
		if (IsValid(Component))
		{
			Component->SetEnableGravity(false);	
			SpawnedVFXComponent->SetVisibility(true, true);
			SpawnedVFXComponent->ReinitializeSystem();
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

	if (bIsMovingItem)
	{
		ReleaseItem();
	}
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
			Component->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
		}

		if (IsValid(SpawnedVFXComponent))
		{
			SpawnedVFXComponent->SetVisibility(false, true);
		}
	}
}
