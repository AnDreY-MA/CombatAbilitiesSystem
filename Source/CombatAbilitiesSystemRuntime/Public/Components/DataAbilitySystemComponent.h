// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DataAbilitySystemComponent.generated.h"


USTRUCT()
struct FCombatAttributeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeSet> AttributeSetType{nullptr};

	UPROPERTY(EditAnywhere, meta = (RequiredAssetDataTags = "RowStructure=/Script/GameplayAbilities.AttributeMetaData"))
	TObjectPtr<UDataTable> DataTable{nullptr};
	
};

UCLASS(ClassGroup="CombatAbilitiesSystem", meta=(BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEM_API UDataAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category="CombatAbilities")
	FGameplayAbilitySpecHandle GrantAbilityOfType(TSubclassOf<UGameplayAbility> InAbilityType, const bool bRemoveAfterActivation);

	void SetupAbilities();
	void SetupAttributes();
	void RemoveAllAbilitiesAndAttributes();

	bool IsUsingAbilityByClass(const TSubclassOf<UGameplayAbility> InAbilityClass) const;

	TArray<UGameplayAbility*> GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> InAbilitySearch) const;

private:
	void GrantAbilitiesAndAttributes();
	
	UFUNCTION()
	void OnPawnControllerChanged(APawn* InPawn, AController* InNewController);

private:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<FCombatAttributeData> DefaultAttributes;
	
	TArray<FGameplayAbilitySpecHandle> DefaultAbilityHandle;

	UPROPERTY(Transient)
	TArray<UAttributeSet*> AddedAttributes;
	
};