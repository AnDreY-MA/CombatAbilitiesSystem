// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameMode.h"

#include "ModularGameMode.generated.h"

class UObject;

/** Pair this with a ModularGameStateBase */
UCLASS(ClassGroup=CombatAbilitiesSystem, Blueprintable)
class COMBATABILITIESSYSTEM_API AModularGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AModularGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

/** Pair this with a ModularGameState */
UCLASS(Blueprintable)
class COMBATABILITIESSYSTEM_API AModularGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AModularGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
