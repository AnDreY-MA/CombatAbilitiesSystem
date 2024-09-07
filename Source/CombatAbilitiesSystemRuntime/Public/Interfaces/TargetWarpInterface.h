// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetWarpInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTargetWarpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATABILITIESSYSTEM_API ITargetWarpInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Target Warp Interface")
	void GetDirection(const FVector& InDirection, USceneComponent*& OutDirectionComponent);
	
};