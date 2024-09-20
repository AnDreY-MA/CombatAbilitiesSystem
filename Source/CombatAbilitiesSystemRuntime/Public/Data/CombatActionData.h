#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "CombatActionData.generated.h"

class UAnimMontage;

USTRUCT(Category="CombatAbilitiesSystem", BlueprintType, Blueprintable)
struct FCombatAnimationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage{nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed{1.0f};
	
};

USTRUCT(Category="CombatAbilitiesSystem",BlueprintType, Blueprintable)
struct COMBATABILITIESSYSTEM_API FCombatActionData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	FGameplayTag TypeAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Setting")
	TArray<FCombatAnimationInfo> Animations;
	
};