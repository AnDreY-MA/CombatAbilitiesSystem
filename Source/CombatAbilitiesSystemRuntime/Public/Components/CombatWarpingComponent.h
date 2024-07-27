// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatWarpingComponent.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEMRUNTIME_API UCombatWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()

public:
	explicit UCombatWarpingComponent(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

	void UpdateWarpTarget();
	void ResetWarpTarget();

private:
	void TryWarpToTarget(AActor* InTarget);

private:
	UPROPERTY(EditDefaultsOnly, Category="Combat Warp")
	FName WarpTargetName;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat Warp")
	float LengthWarp;

	UPROPERTY(EditDefaultsOnly, Category="Combat Warp")
	float RadiusSphere;

	UPROPERTY(EditDefaultsOnly, Category="Combat Warp")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat Warp")
	TEnumAsByte<EDrawDebugTrace::Type> DebugTraceType;
};