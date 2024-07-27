// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularsActors/ModularGameMode.h"

#include "ModularsActors/ModularGameState.h"
#include "ModularsActors/ModularPawn.h"
#include "ModularsActors/ModularPlayerController.h"
#include "ModularsActors/ModularPlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModularGameMode)

AModularGameModeBase::AModularGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AModularGameStateBase::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	PlayerStateClass = AModularPlayerState::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}

AModularGameMode::AModularGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AModularGameState::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	PlayerStateClass = AModularPlayerState::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}

