// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TraceNotifyState.generated.h"

/**
 * 
 */
UCLASS(ClassGroup="CombatAbilitiesSystem")
class COMBATABILITIESSYSTEM_API UTraceNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	class UCombatTraceComponent* TryGetTraceComponent(const AActor* InOwner);

private:
	UPROPERTY(EditAnywhere, Category="TraceState")
	bool bChangeSockets{ false };

	UPROPERTY(EditAnywhere, Category = "TraceState", meta = (EditCondition = "bChangeSockets"))
	FName StartSocketName;

	UPROPERTY(EditAnywhere, Category = "TraceState", meta = (EditCondition = "bChangeSockets"))
	FName EndSocketName;
};
