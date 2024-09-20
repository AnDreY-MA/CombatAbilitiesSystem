// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ModularPawn.generated.h"

class UObject;

UCLASS(ClassGroup="CombatAbilitiesSystem", Blueprintable)
class COMBATABILITIESSYSTEM_API AModularPawn : public APawn
{
	GENERATED_BODY()

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

};
