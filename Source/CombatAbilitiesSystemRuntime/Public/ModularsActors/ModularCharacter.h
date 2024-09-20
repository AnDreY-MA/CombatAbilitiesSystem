// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "ModularCharacter.generated.h"

class UObject;


UCLASS(ClassGroup=CombatAbilitiesSystem, Blueprintable)
class COMBATABILITIESSYSTEM_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;
	
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};