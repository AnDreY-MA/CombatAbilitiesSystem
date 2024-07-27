	// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameState.h"

#include "ModularGameState.generated.h"

class UObject;

/** Pair this with a ModularGameModeBase */
UCLASS(Blueprintable)
class COMBATABILITIESSYSTEMRUNTIME_API AModularGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface
};


/** Pair this with a ModularGameState */
UCLASS(ClassGroup=CombatAbilitiesSystem, Blueprintable)
class COMBATABILITIESSYSTEMRUNTIME_API AModularGameState : public AGameState
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

protected:
	//~ Begin AGameState interface
	virtual void HandleMatchHasStarted() override;
	//~ Begin AGameState interface
};
