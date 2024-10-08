// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatTraceComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatTraceComponentInterface : public UInterface
{
	GENERATED_BODY()
};


class COMBATABILITIESSYSTEM_API ICombatTraceComponentInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CombatTraceComponent Interface")
	class UCombatTraceComponent* GetCombatTraceComponent() const;

};