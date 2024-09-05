// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ComboWindowAnimNotifyState.generated.h"

UCLASS()
class COMBATABILITIESSYSTEMRUNTIME_API UComboWindowAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	explicit UComboWindowAnimNotifyState(const FObjectInitializer& InInitializer = FObjectInitializer::Get());

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category="Combo")
	bool bEndCombo;
	
	UPROPERTY()
	TObjectPtr<UObject> CombatComponent;

};