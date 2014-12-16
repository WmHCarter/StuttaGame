// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "StuttaGameEnums.h"
#include "StuttaGameCharacter.generated.h"

UCLASS(config=Game)
class AStuttaGameCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	EDirection currentDirection;
	EDirection previousDirection;

	/** Is the character attacking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterState)
	ECharacterState characterState;

	/** Time that the character takes to turn to attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState)
	float baseWindupTime;

	/** Time that the character takes to turn to attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState)
	float windupReduction;

	/** Time that the character takes to turn to flee */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState)
	float baseWinddownTime;

	/** Time that the character takes to turn to flee */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState)
	float winddownReduction;

	/** Number of attacks per second */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState)
	float baseAttackSpeed;

	/** Number of attacks per second */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CharacterState)
	float bonusAttackSpeed;

protected:

	virtual void Tick(float DeltaSeconds) OVERRIDE;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

};

