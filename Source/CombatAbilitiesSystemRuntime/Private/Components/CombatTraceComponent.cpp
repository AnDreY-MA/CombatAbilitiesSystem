// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatTraceComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CollisionDebugDrawingPublic.h"
#include "CombatAbilitiesSystemRuntimeModule.h"


DECLARE_CYCLE_STAT(TEXT("DoTrace"), STAT_DoTraceTest, STATGROUP_CombatTraceComponent);
DECLARE_CYCLE_STAT(TEXT("RunStartTrace"), STAT_RunStartTrace, STATGROUP_CombatTraceComponent);

UCombatTraceComponent::UCombatTraceComponent(const FObjectInitializer& InInitializer)
	: Super(InInitializer), CapsuleRadius(20), CapsuleHalfHeight(40)
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

void UCombatTraceComponent::ChangeTracingComponent(USceneComponent* InSceneComponent)
{
	TracingComponent = InSceneComponent;
}

void UCombatTraceComponent::DoTrace() const
{
	SCOPE_CYCLE_COUNTER(STAT_DoTraceTest);

	auto* World {GetWorld()};
	const FVector Start{TracingComponent->GetSocketLocation(NameSocketStart)};
	const FVector End{TracingComponent->GetSocketLocation(NameSocketEnd)};
	const FQuat Rot = TracingComponent->GetSocketQuaternion(NameSocketStart);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

#if !UE_BUILD_SHIPPING || WITH_EDITORONLY_DATA
	TryDrawDebug(Start, End, Rot);
#endif
	
	if(TraceStyleType == ECombatCollisionStyleType::Line)
	{
		World->AsyncLineTraceByChannel(EAsyncTraceType::Single, Start, End, TraceChannel, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
	}
	else
	{
		if(ShapeType == ECombatCollisionShapeType::Capsule)
		{
			GetWorld()->AsyncSweepByChannel(EAsyncTraceType::Single, Start, End, Rot, TraceChannel, FCollisionShape::MakeCapsule(20, 40), CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam, &TraceDelegate);
		}
	}
	
}

void UCombatTraceComponent::OnTraceDelegate(const FTraceHandle& InTraceHandle, FTraceDatum& InTraceDatum)
{
	if(InTraceDatum.OutHits.Num())
	{
		FGameplayEventData Payload;
		Payload.Instigator = GetOwner();
		Payload.Target = InTraceDatum.OutHits[0].GetActor();
		Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(InTraceDatum.OutHits[0].GetActor());
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EventHitTag, Payload);
		UE_LOG(LogCombatAbilitySystem, Display, TEXT("Hit Actor %s"), *InTraceDatum.OutHits[0].GetActor()->GetName());
	}
}

#if !UE_BUILD_SHIPPING || WITH_EDITORONLY_DATA
void UCombatTraceComponent::TryDrawDebug(const FVector& InStart, const FVector& InEnd, const FQuat& InRot) const
{
	if (!bShouldDebug) return;

	if(TraceStyleType == ECombatCollisionStyleType::Line)
	{
		DrawDebugLine(GetWorld(), InStart, InEnd, ColorDebug, false, LifeTimeDebugLine);
	}
	else if(TraceStyleType == ECombatCollisionStyleType::Sweep)
	{
		if(ShapeType == ECombatCollisionShapeType::Capsule)
		{
			DrawCapsuleSweeps(GetWorld(), InStart, InEnd, 20, 40, InRot, {}, LifeTimeDebugLine);
		}
		else if(ShapeType == ECombatCollisionShapeType::Sphere)
		{
			DrawSphereSweeps(GetWorld(), InStart, InEnd, 30, {}, LifeTimeDebugLine);
		}
	}
}
# endif