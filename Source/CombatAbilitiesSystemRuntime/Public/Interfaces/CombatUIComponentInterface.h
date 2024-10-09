// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatUIComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatUIComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATABILITIESSYSTEM_API ICombatUIComponentInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat UI Component Interface")
	class UCombatUIComponent* GetCombatUIComponent() const;

};