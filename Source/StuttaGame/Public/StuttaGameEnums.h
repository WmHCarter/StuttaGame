#pragma once

#include "StuttaGameEnums.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Left,
	Right,
	Neutral
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Turning,
	NotAttacking,
	TurnToAttack,
	TurnToMove,
	Attacking,
	Special
};

