// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatWarpingInterface.generated.h"

UINTERFACE(Category="CombatAbilitiesSystem",MinimalAPI)
class UCombatWarpingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATABILITIESSYSTEM_API ICombatWarpingInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Warping Interface")
	void UpdateWarpTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Combat Warping Interface")
	void ResetWarpTarget();
	
};