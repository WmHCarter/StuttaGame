// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "StuttaGame.h"
#include "StuttaGameCharacter.h"

AStuttaGameCharacter::AStuttaGameCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Face in the direction we are moving..
	CharacterMovement->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->GravityScale = 2.f;
	CharacterMovement->AirControl = 0.80f;
	CharacterMovement->JumpZVelocity = 1000.f;
	CharacterMovement->GroundFriction = 3.f;
	CharacterMovement->MaxWalkSpeed = 600.f;
	CharacterMovement->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	currentDirection = EDirection::Neutral;
	characterState = ECharacterState::NotAttacking;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStuttaGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveRight", this, &AStuttaGameCharacter::MoveRight);

	InputComponent->BindTouch(IE_Pressed, this, &AStuttaGameCharacter::TouchStarted);
}

void AStuttaGameCharacter::MoveRight(float Value)
{
	if (Value != 0)
	{
		previousDirection = currentDirection;
		if (Value == 1.f)
		{
			currentDirection = EDirection::Right;
		}
		else if (Value == -1.f)
		{
			currentDirection = EDirection::Left;
		}

		if (previousDirection != currentDirection)
		{
			if (characterState == ECharacterState::NotAttacking)
			{
				characterState = ECharacterState::TurnToAttack;
			}
			else if (characterState == ECharacterState::Attacking)
			{
				characterState = ECharacterState::TurnToMove;
			}
		}

		if (previousDirection == currentDirection)
		{
			if (characterState == ECharacterState::Attacking)
			{
				characterState = ECharacterState::TurnToMove;
			}
		}
	}
}

void AStuttaGameCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{

}

void AStuttaGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If character is turning to attack, rotate character
	if (characterState == ECharacterState::TurnToAttack)
	{
		if (currentDirection == EDirection::Left)
		{
			SetActorRotation(FRotator(0, FMath::FixedTurn(GetActorRotation().Yaw, 90, 15), 0));
		}
		else if (currentDirection == EDirection::Right)
		{
			SetActorRotation(FRotator(0, FMath::FixedTurn(GetActorRotation().Yaw, -90, -15), 0));
		}

		if (GetActorRotation().Yaw >= 89.9f || GetActorRotation().Yaw <= -89.9f)
		{
			characterState = ECharacterState::Attacking;
		}
	}

	// If character is turning to move, rotate character
	if (characterState == ECharacterState::TurnToMove)
	{
		if (currentDirection == EDirection::Left)
		{
			SetActorRotation(FRotator(0, FMath::FixedTurn(GetActorRotation().Yaw, 90, 90), 0));
		}
		else if (currentDirection == EDirection::Right)
		{
			SetActorRotation(FRotator(0, FMath::FixedTurn(GetActorRotation().Yaw, -90, -90), 0));
		}

		if (GetActorRotation().Yaw >= 89.9f || GetActorRotation().Yaw <= -89.9f)
		{
			characterState = ECharacterState::NotAttacking;
		}
	}

	if (currentDirection != EDirection::Neutral)
	{
		if (characterState == ECharacterState::Attacking)
		{
			//Stand still and start attacking
		}

		if (characterState == ECharacterState::NotAttacking)
		{
			if (currentDirection == EDirection::Left)
				AddMovementInput(FVector(0.f, -1.f, 0.f), -1.f);
			if (currentDirection == EDirection::Right)
				AddMovementInput(FVector(0.f, -1.f, 0.f), 1.f);
		}
	}
}

