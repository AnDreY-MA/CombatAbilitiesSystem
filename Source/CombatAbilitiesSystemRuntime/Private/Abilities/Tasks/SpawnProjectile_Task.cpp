// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Tasks/SpawnProjectile_Task.h"

#include "CombatAbilitiesSystemModule.h"
#include "Actors/CombatProjectile.h"
#include "Logging/StructuredLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SpawnProjectile_Task)

USpawnProjectile_Task* USpawnProjectile_Task::SpawnProjectileTask(UGameplayAbility* InOwningAbility,
	const FName& TaskInstanceName, TSubclassOf<ACombatProjectile> InProjectileClass, const FDamageEffectData& InDamageData, const FTransform& InTransformSpawn)
{
	USpawnProjectile_Task* SpawnTask {NewAbilityTask<USpawnProjectile_Task>(InOwningAbility, TaskInstanceName)};
	SpawnTask->ProjectileClass = InProjectileClass;
	SpawnTask->ProjectileTransform = InTransformSpawn;
	SpawnTask->DamageEffectData = InDamageData;
	
	return SpawnTask;
}

void USpawnProjectile_Task::Activate()
{
	Super::Activate();

	auto* ProjectileSpawned {GetWorld()->SpawnActorDeferred<ACombatProjectile>(ProjectileClass, ProjectileTransform, GetOwnerActor())};
	ProjectileSpawned->SetupProjectile(DamageEffectData);
	ProjectileSpawned->FinishSpawning(ProjectileTransform);

	if(ShouldBroadcastAbilityTaskDelegates())
	{
		if(ProjectileSpawned)
		{
			OnSpawnProjectile.Broadcast(ProjectileSpawned);
		}
		else
		{
			OnSpawnFailed.Broadcast(ProjectileSpawned);
		}
	}
	
}