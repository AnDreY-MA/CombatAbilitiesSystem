#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "CombatActionData.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType, Blueprintable)
struct COMBATABILITIESSYSTEMRUNTIME_API FCombatActionData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Setting")
	FGameplayTag TypeAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setting")
	TArray<TObjectPtr<UAnimMontage>> Animations;
	
};