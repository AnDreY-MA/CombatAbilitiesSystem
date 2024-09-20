// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RangedAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "CombatAbilitiesSystemModule.h"
#include "Abilities/Tasks/PlayMontageAndWaitForEvent.h"
#include "Abilities/Tasks/SpawnProjectile_Task.h"
#include "Actors/CombatProjectile.h"
#include "Interfaces/CombatComponentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/StructuredLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RangedAttackAbility)


void URangedAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	if(auto* AbilityComponent{ActorInfo->AvatarActor.Get()->FindComponentByInterface<UCombatComponentInterface>()}; AbilityComponent)
	{
		ICombatComponentInterface::Execute_IncrementComboIndex(AbilityComponent);

		const auto [Montage, Speed] = ICombatComponentInterface::Execute_GetMontageAction(AbilityComponent, AbilityTags.First())[0];
		auto* MontageTask {UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, Montage, FGameplayTagContainer(ShootEvent), Speed)};
		MontageTask->OnCancelled.AddDynamic(this, &UCombatAbilityBase::OnMontageCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &UCombatAbilityBase::OnMontageCancelled);
		MontageTask->EventReceived.AddDynamic(this, &UCombatAbilityBase::OnEventReceived);	
		MontageTask->ReadyForActivation();
		//PlayMontageWaitEvents(Montage.Get(), FGameplayTagContainer(ShootEvent), Speed, NAME_None, false);
	}

}

void URangedAttackAbility::OnEventReceived_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
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
	const FRotator AvatarRotation {AvatarActor->GetActorRotation()};
	const FVector SocketSpawnLocation {ComponentFromSpawn->GetSocketLocation(NameSocketFromShoot) + UKismetMathLibrary::GreaterGreater_VectorRotator(ProjectileOffset, AvatarRotation)};
	
	const FTransform ProjectileTransform { FTransform(AvatarRotation, SocketSpawnLocation, FVector(1.0, 1.0, 1.0)) };
	//ProjectileTransform.SetLocation(FVector(SocketSpawnLocation.X + 50.0, SocketSpawnLocation.Y, SocketSpawnLocation.Z));

	const FDamageEffectData& DamageData { MakeDamageData() };
	auto* ProjectileTask {USpawnProjectile_Task::SpawnProjectileTask(this, NAME_None, ProjectileClass, DamageData, ProjectileTransform)};
	ProjectileTask->OnSpawnProjectile.AddDynamic(this, &URangedAttackAbility::OnProjectileSpawn);
	ProjectileTask->ReadyForActivation();
	
}

void URangedAttackAbility::OnProjectileSpawn_Implementation(ACombatProjectile* SpawnedProjectile)
{
	SpawnedProjectile->OnActorHit.AddDynamic(this, &URangedAttackAbility::HitProjectile);
}

void URangedAttackAbility::HitProjectile(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
	const FHitResult& Hit)
{
	UE_LOGFMT(LogCombatAbilitySystem, Warning, "Hit {0}", OtherActor->GetName());
}