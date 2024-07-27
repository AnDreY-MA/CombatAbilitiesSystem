// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"

#include "ModularPlayerController.generated.h"


UCLASS(ClassGroup=CombatAbilitiesSystem, Blueprintable)
class COMBATABILITIESSYSTEMRUNTIME_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void ReceivedPlayer() override;
	
	virtual void PlayerTick(float DeltaTime) override;
	
};