// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageAbility.h"
#include "RangedAttackAbility.generated.h"

class ACombatProjectile;

UCLASS(ClassGroup="CombatAbilitiesSystem", Abstract)
class COMBATABILITIESSYSTEM_API URangedAttackAbility : public UDamageAbility
{
	GENERATED_BODY()

public:
	virtual void OnEventReceived_Implementation(FGameplayTag EventTag, FGameplayEventData EventData) override;


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UFUNCTION()
	void HitProjectile(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileSpawn(ACombatProjectile* SpawnedProjectile);

	UFUNCTION(BlueprintCallable)
	virtual void Shoot();
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category="Combat|Events")
	FGameplayTag ShootEvent;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat|RangedAttack")
	FName NameSocketFromShoot;
	UPROPERTY(EditDefaultsOnly, Category="Combat|RangedAttack")
	TSubclassOf<ACombatProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category="Combat|RangedAttack")
	FVector ProjectileOffset;
	
};