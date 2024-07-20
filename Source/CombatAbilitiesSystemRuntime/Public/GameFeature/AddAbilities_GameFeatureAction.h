// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeature_WorldActionBase.h"
#include "GameplayAbilitySpec.h"
#include "Components/GameFrameworkComponentManager.h"
#include "AddAbilities_GameFeatureAction.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FCombatAbilityMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbility> Ability;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> InputAction;

};

USTRUCT(BlueprintType)
struct FCombatAttributesMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UAttributeSet> Attribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> AttributeData;
	
};

USTRUCT()
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<FCombatAbilityMapping> GrantedAbilities;
	
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<FCombatAttributesMapping> GrantedAttributes;
};

UCLASS(DisplayName="Add Combat Abilities")
class COMBATABILITIESSYSTEMRUNTIME_API UAddAbilities_GameFeatureAction : public UGameFeature_WorldActionBase
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	
private:
	virtual void AddToWorld(const FWorldContext& InWorldContext) override;

	void RemoveActorAbilities(AActor* InActor);
	void Reset();
	void HandleActorExtension(AActor* InActor, FName InEventName, const int32 EntryIndex);
	void AddActorAbilities(AActor* InActor, const FGameFeatureAbilitiesEntry& AbilitiesEntry);

	template<class ComponentType>
	ComponentType* FindOrAddComponentForActor(const AActor* InActor, const FGameFeatureAbilitiesEntry& InAbilitiesEntry)
	{
		return Cast<ComponentType>(FindOrAddComponentForActor(ComponentType::StaticClass(), InActor, InAbilitiesEntry));
	}

	UActorComponent* FindOrAddComponentForActor(UClass* InComponentType, const AActor* InActor, const FGameFeatureAbilitiesEntry& InAbilitiesEntry);

private:
	UPROPERTY(EditAnywhere, Category="Abilities", meta=(AllowPrivateAccess="true", TitleProperty="ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesEntry> AbilitiesList;
	
	struct FActorExtensions
	{
		TArray<FGameplayAbilitySpecHandle> Abilities;
		TArray<UAttributeSet*> Attributes; 
	};
	
	TMap<TObjectPtr<AActor>, FActorExtensions> ActiveExtensions;
	
	TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;

};