// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RangedAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "CombatAbilitiesSystemModule.h"
#include "Abilities/Tasks/SpawnProjectile_Task.h"
#include "Actors/CombatProjectile.h"
#include "Interfaces/CombatComponentInterface.h"
#include "Logging/StructuredLog.h"

void URangedAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	if(auto* AbilityComponent{ActorInfo->AvatarActor.Get()->FindComponentByInterface<UCombatComponentInterface>()}; AbilityComponent)
	{
		ICombatComponentInterface::Execute_IncrementComboIndex(AbilityComponent);

		const auto [Montage, Speed] = ICombatComponentInterface::Execute_GetComboMontageAction(AbilityComponent, AbilityTags.First());
		PlayMontageWaitEvents(Montage, FGameplayTagContainer(ShootEvent), Speed);
	}

}

void URangedAttackAbility::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if(EventTag == ShootEvent)
	{
		Shoot();
	}
	
}


void URangedAttackAbility::Shoot()
{
	const auto* AvatarActor{CurrentActorInfo->AvatarActor.Get()};
	const auto* ComponentFromSpawn {AvatarActor->FindComponentByClass<USkeletalMeshComponent>()};
	const FVector SocketSpawnLocation {ComponentFromSpawn->GetSocketLocation(NameSocketFromShoot)};
	
	FTransform ProjectileTransform(AvatarActor->GetActorRotation(),
		FVector(SocketSpawnLocation.X + 50.0, SocketSpawnLocation.Y, SocketSpawnLocation.Z), FVector(1.0, 1.0, 1.0));
	ProjectileTransform.SetLocation(FVector(SocketSpawnLocation.X + 50.0, SocketSpawnLocation.Y, SocketSpawnLocation.Z));

	const FDamageEffectData& DamageData {MakeDamageData()};
	auto* ProjectileTask {USpawnProjectile_Task::SpawnProjectileTask(this, NAME_None, ProjectileClass, DamageData, ProjectileTransform)};
	ProjectileTask->OnSpawnProjectile.AddDynamic(this, &URangedAttackAbility::OnProjectileSpawn);
	ProjectileTask->ReadyForActivation();
	
}

void URangedAttackAbility::OnProjectileSpawn(ACombatProjectile* SpawnedProjectile)
{
	SpawnedProjectile->OnActorHit.AddDynamic(this, &URangedAttackAbility::HitProjectile);
}

void URangedAttackAbility::HitProjectile(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	UE_LOGFMT(LogCombatAbilitySystem, Warning, "Hit {0}", OtherActor->GetName());
}