// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbilitiesCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "TelekinesisComponent.h"
#include "BlinkComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGameplayAbilitiesCharacter

AGameplayAbilitiesCharacter::AGameplayAbilitiesCharacter()
{
	CapsuleRadius = 55.0f;
	CapsuleHalfHeight = 96.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	POV_Origin = FVector(-10.f, 0.f, 60.f);
	FirstPersonCameraComponent->SetRelativeLocation(POV_Origin); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	BlinkComponent = CreateDefaultSubobject<UBlinkComponent>(TEXT("BlinkComponent"));
	TelekinesisComponent = CreateDefaultSubobject<UTelekinesisComponent>(TEXT("TelekinesisComponent"));

	jumpCount = 0;
	PushForceOnGround = GetCharacterMovement()->PushForceFactor;
}

void AGameplayAbilitiesCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGameplayAbilitiesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameplayAbilitiesCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameplayAbilitiesCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}


void AGameplayAbilitiesCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGameplayAbilitiesCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

bool AGameplayAbilitiesCharacter::CanJumpInternal_Implementation() const
{
	Super::CanJumpInternal_Implementation();

	// Double jump
	return jumpCount < 2;
}

void AGameplayAbilitiesCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (jumpCount < 2)
	{
		jumpCount++;
	}

	GetCharacterMovement()->PushForceFactor = PushForceInAir;
}

void AGameplayAbilitiesCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	jumpCount = 0;
	GetCharacterMovement()->PushForceFactor = PushForceOnGround;
}

void AGameplayAbilitiesCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	OnCharacterDeath.Broadcast();
}
