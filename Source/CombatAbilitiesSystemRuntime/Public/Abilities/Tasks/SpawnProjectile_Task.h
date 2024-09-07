// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/DamageData.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SpawnProjectile_Task.generated.h"

class ACombatProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnProjectile, ACombatProjectile*, SpawnedProjectile);

UCLASS(NotBlueprintable, NotBlueprintType)
class COMBATABILITIESSYSTEM_API USpawnProjectile_Task final : public UAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="CombatAbility|Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static USpawnProjectile_Task* SpawnProjectileTask(UGameplayAbility* InOwningAbility, const FName& TaskInstanceName,
		TSubclassOf<ACombatProjectile> InProjectileClass, const FDamageEffectData& InDamageData, const FTransform& InTransformSpawn);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnSpawnProjectile OnSpawnProjectile;

	UPROPERTY(BlueprintAssignable)
	FOnSpawnProjectile OnSpawnFailed;
	
private:
	UPROPERTY()
	TSubclassOf<ACombatProjectile> ProjectileClass;

	UPROPERTY()
	FDamageEffectData DamageEffectData;
	
	UPROPERTY()
	FTransform ProjectileTransform;
	
};