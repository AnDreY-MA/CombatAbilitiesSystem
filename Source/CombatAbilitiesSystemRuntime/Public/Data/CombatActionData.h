#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "CombatActionData.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType, Blueprintable)
struct FCombatAnimationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage{nullptr};

	UPROPERTY(EditDefaultsOnly)
	float Speed{1.0f};
	
};

USTRUCT(BlueprintType, Blueprintable)
struct COMBATABILITIESSYSTEM_API FCombatActionData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Setting")
	FGameplayTag TypeAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setting")
	TArray<FCombatAnimationInfo> Animations;
	
};