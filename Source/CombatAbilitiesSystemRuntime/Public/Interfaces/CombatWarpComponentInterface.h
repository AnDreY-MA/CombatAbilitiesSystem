// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatWarpComponentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCombatWarpComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATABILITIESSYSTEM_API ICombatWarpComponentInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat WarpComponent Interface")
	class UCombatWarpingComponent* GetCombatWarpingComponent() const;
};
