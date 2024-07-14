// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatTraceComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CollisionDebugDrawingPublic.h"
#include "CombatAbilitiesSystemRuntimeModule.h"


DECLARE_CYCLE_STAT(TEXT("DoTrace"), STAT_DoTraceTest, STATGROUP_CombatTraceComponent);
DECLARE_CYCLE_STAT(TEXT("RunStartTrace"), STAT_RunStartTrace, STATGROUP_CombatTraceComponent);

UCombatTraceComponent::UCombatTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCombatTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	TraceDelegate.BindUObject(this, &UCombatTraceComponent::OnTraceDelegate);

}

void UCombatTraceComponent::RunAsyncTaskStartTrace()
{
	SCOPE_CYCLE_COUNTER(STAT_RunStartTrace);

	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &UCombatTraceComponent::DoTrace, TraceInterval, true);
}


void UCombatTraceComponent::ActivateTrace()
{
	RunAsyncTaskStartTrace();
	
}

void UCombatTraceComponent::DeactivateTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
	
}

void UCombatTraceComponent::DoAsyncLineTraceFromLocation(const FVector& InStart, const FVector& InEnd) const
{
	if (bShouldDebug)
	{
		//DrawCapsuleSweeps(GetWorld(), Start, End, 20, 40, Rot, {}, LifeTimeDebugLine);
		DrawDebugLine(GetWorld(), InStart, InEnd, ColorDebug, false, LifeTimeDebugLine);

	}
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->AsyncLineTraceByChannel(EAsyncTraceType::Single, InStart, InEnd, TraceChannel, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
}

void UCombatTraceComponent::ChangeTracingComponent(USceneComponent* InSceneComponent)
{
	TracingComponent = InSceneComponent;
}

void UCombatTraceComponent::DoTrace() const
{
	SCOPE_CYCLE_COUNTER(STAT_DoTraceTest);

	const FVector Start{TracingComponent->GetSocketLocation(NameSocketStart)};
	const FVector End{TracingComponent->GetSocketLocation(NameSocketEnd)};
	FQuat Rot = TracingComponent->GetSocketQuaternion(NameSocketStart);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	DoAsyncLineTraceFromLocation(Start, End);
	
	/*#if !UE_BUILD_SHIPPING || WITH_EDITORONLY_DATA

	if (bShouldDebug)
	{
		//DrawCapsuleSweeps(GetWorld(), Start, End, 20, 40, Rot, {}, LifeTimeDebugLine);
		DrawDebugLine(GetWorld(), Start, End, ColorDebug, false, LifeTimeDebugLine);

	}
	#endif
	//GetWorld()->AsyncSweepByChannel(EAsyncTraceType::Single, Start, End, Rot, TraceChannel, FCollisionShape::MakeCapsule(20, 40), CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
	GetWorld()->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End, TraceChannel, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
	*/
}

void UCombatTraceComponent::OnTraceDelegate(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum)
{
	if(InTraceDatum.OutHits.Num())
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetOwner();
		Payload.Target = InTraceDatum.OutHits[0].GetActor();
		Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(InTraceDatum.OutHits[0].GetActor());
		UE_LOG(LogTemp, Warning, TEXT("Owner = %s"), *GetOwner()->GetName());
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EventHitTag, Payload);
		UE_LOG(LogCombatAbilitySystem, Display, TEXT("Hit Actor %s"), *InTraceDatum.OutHits[0].GetActor()->GetName());
	}
}