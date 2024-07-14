
// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeature/AddAbilities_GameFeatureAction.h"

#include "AbilitySystemComponent.h"
#include "CombatAbilitiesSystemRuntimeModule.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"
#include "Input/AbilityInputBindingComponent.h"


#define LOCTEXT_NAMESPACE "<PROJECT_NAME>Features"

//////////////////////////////////////////////////////////////////////
// UAddAbilities_GameFeatureAction

void UAddAbilities_GameFeatureAction::OnGameFeatureActivating()
{
	if (!ensureAlways(ActiveExtensions.IsEmpty()) ||
		!ensureAlways(ComponentRequests.IsEmpty()))
	{
		Reset();
	}
	Super::OnGameFeatureActivating();
}

void UAddAbilities_GameFeatureAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	Reset();
}

#if WITH_EDITORONLY_DATA
void UAddAbilities_GameFeatureAction::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		auto AddBundleAsset = [&AssetBundleData](const FSoftObjectPath& SoftObjectPath)
		{
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, SoftObjectPath);
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, SoftObjectPath);
		};

		for (const FGameFeatureAbilitiesEntry& Entry : AbilitiesList)
		{
			for (const FCombatAbilityMapping& Ability : Entry.GrantedAbilities)
			{
				AddBundleAsset(Ability.Ability.ToSoftObjectPath());
				if (!Ability.InputAction.IsNull())
				{
					AddBundleAsset(Ability.InputAction.ToSoftObjectPath());
				}
			}

			for (const FCombatAttributesMapping& Attributes : Entry.GrantedAttributes)
			{
				AddBundleAsset(Attributes.Attribute.ToSoftObjectPath());
				if (!Attributes.AttributeData.IsNull())
				{
					AddBundleAsset(Attributes.AttributeData.ToSoftObjectPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UAddAbilities_GameFeatureAction::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const FGameFeatureAbilitiesEntry& Entry : AbilitiesList)
	{
		if (Entry.ActorClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.GrantedAbilities.IsEmpty() && Entry.GrantedAttributes.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNoAddOns", "Empty GrantedAbilities and GrantedAttributes at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		int32 AbilityIndex = 0;
		for (const FCombatAbilityMapping& Ability : Entry.GrantedAbilities)
		{
			if (Ability.Ability.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullAbility", "Null AbilityType at index {0} in AbilitiesList[{1}].GrantedAbilities"), FText::AsNumber(AbilityIndex), FText::AsNumber(EntryIndex)));
			}
			++AbilityIndex;
		}

		int32 AttributesIndex = 0;
		for (const FCombatAttributesMapping& Attributes : Entry.GrantedAttributes)
		{
			if (Attributes.Attribute.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullAttributeSet", "Null AttributeSetType at index {0} in AbilitiesList[{1}].GrantedAttributes"), FText::AsNumber(AttributesIndex), FText::AsNumber(EntryIndex)));
			}
			++AttributesIndex;
		}

		++EntryIndex;
	}

	return Result;

	return EDataValidationResult::NotValidated;
}
#endif

void UAddAbilities_GameFeatureAction::AddToWorld(const FWorldContext& WorldContext)
{
	UE_LOG(LogCombatAbilitySystem, Warning, TEXT("ADD TO WORLD ABILITY"));
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{			
			int32 EntryIndex = 0;
			for (const FGameFeatureAbilitiesEntry& Entry : AbilitiesList)
			{
				if (!Entry.ActorClass.IsNull())
				{
					UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this, &UAddAbilities_GameFeatureAction::HandleActorExtension, EntryIndex);
					TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMan->AddExtensionHandler(Entry.ActorClass, AddAbilitiesDelegate);

					ComponentRequests.Add(ExtensionRequestHandle);
					EntryIndex++;
				}
			}
		}
	}
}

void UAddAbilities_GameFeatureAction::Reset()
{
	while (!ActiveExtensions.IsEmpty())
	{
		auto ExtensionIt = ActiveExtensions.CreateIterator();
		RemoveActorAbilities(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UAddAbilities_GameFeatureAction::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (AbilitiesList.IsValidIndex(EntryIndex))
	{
		const FGameFeatureAbilitiesEntry& Entry = AbilitiesList[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
		{
			RemoveActorAbilities(Actor);
		}
		else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddActorAbilities(Actor, Entry);
		}
	}
}

void UAddAbilities_GameFeatureAction::AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry)
{
	UE_LOG(LogCombatAbilitySystem, Error, TEXT("Added ability"), *Actor->GetPathName());
	if (UAbilitySystemComponent* AbilitySystemComponent = FindOrAddComponentForActor<UAbilitySystemComponent>(Actor, AbilitiesEntry))
	{
		FActorExtensions AddedExtensions;
		AddedExtensions.Abilities.Reserve(AbilitiesEntry.GrantedAbilities.Num());
		AddedExtensions.Attributes.Reserve(AbilitiesEntry.GrantedAttributes.Num());

		for (const FCombatAbilityMapping& Ability : AbilitiesEntry.GrantedAbilities)
		{
			if (!Ability.Ability.IsNull())
			{
				FGameplayAbilitySpec NewAbilitySpec(Ability.Ability.LoadSynchronous());
				FGameplayAbilitySpecHandle AbilityHandle = AbilitySystemComponent->GiveAbility(NewAbilitySpec);

				if (!Ability.InputAction.IsNull())
				{
					UAbilityInputBindingComponent* InputComponent = FindOrAddComponentForActor<UAbilityInputBindingComponent>(Actor, AbilitiesEntry);
					if (InputComponent)
					{
						InputComponent->SetInputBinding(Ability.InputAction.LoadSynchronous(), AbilityHandle);
					}
					else
					{
						 UE_LOG(LogCombatAbilitySystem, Error, TEXT("Failed to find/add an ability input binding component to '%s' -- are you sure it's a pawn class?"), *Actor->GetPathName());
					}
				}

				AddedExtensions.Abilities.Add(AbilityHandle);
			}
		}

		for (const FCombatAttributesMapping& Attributes : AbilitiesEntry.GrantedAttributes)
		{
			if (!Attributes.Attribute.IsNull())
			{
				TSubclassOf<UAttributeSet> SetType = Attributes.Attribute.LoadSynchronous();
				if (SetType)
				{
					UAttributeSet* NewSet = NewObject<UAttributeSet>(AbilitySystemComponent, SetType);
					if (!Attributes.AttributeData.IsNull())
					{
						UDataTable* InitData = Attributes.AttributeData.LoadSynchronous();
						if (InitData)
						{
							NewSet->InitFromMetaDataTable(InitData);
						}
					}

					AddedExtensions.Attributes.Add(NewSet);
					AbilitySystemComponent->AddAttributeSetSubobject(NewSet);
				}
			}
		}

		ActiveExtensions.Add(Actor, AddedExtensions);
	}
	else
	{
		UE_LOG(LogCombatAbilitySystem, Error, TEXT("Failed to find/add an ability component to '%s'. Abilities will not be granted."), *Actor->GetPathName());
	}
}

UActorComponent* UAddAbilities_GameFeatureAction::FindOrAddComponentForActor(UClass* InComponentType,
	const AActor* InActor, const FGameFeatureAbilitiesEntry& InAbilitiesEntry)
{
	UActorComponent* Component = InActor->FindComponentByClass(InComponentType);
	
	bool bMakeComponentRequest = (Component == nullptr);
	if (Component)
	{
		// Check to see if this component was created from a different `UGameFrameworkComponentManager` request.
		// `Native` is what `CreationMethod` defaults to for dynamically added components.
		if (Component->CreationMethod == EComponentCreationMethod::Native)
		{
			// Attempt to tell the difference between a true native component and one created by the GameFrameworkComponent system.
			// If it is from the UGameFrameworkComponentManager, then we need to make another request (requests are ref counted).
			UObject* ComponentArchetype = Component->GetArchetype();
			bMakeComponentRequest = ComponentArchetype->HasAnyFlags(RF_ClassDefaultObject);
		}
	}

	if (bMakeComponentRequest)
	{
		UWorld* World = InActor->GetWorld();
		UGameInstance* GameInstance = World->GetGameInstance();

		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			TSharedPtr<FComponentRequestHandle> RequestHandle = ComponentMan->AddComponentRequest(InAbilitiesEntry.ActorClass, InComponentType);
			ComponentRequests.Add(RequestHandle);
		}

		if (!Component)
		{
			Component = InActor->FindComponentByClass(InComponentType);
			ensureAlways(Component);
		}
	}

	return Component;
}

void UAddAbilities_GameFeatureAction::RemoveActorAbilities(AActor* Actor)
{
	if (FActorExtensions* ActorExtensions = ActiveExtensions.Find(Actor))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = Actor->FindComponentByClass<UAbilitySystemComponent>())
		{
			for (UAttributeSet* AttribSetInstance : ActorExtensions->Attributes)
			{
				AbilitySystemComponent->GetSpawnedAttributes_Mutable().Remove(AttribSetInstance);
			}

			UAbilityInputBindingComponent* InputComponent = Actor->FindComponentByClass<UAbilityInputBindingComponent>();
			for (FGameplayAbilitySpecHandle AbilityHandle : ActorExtensions->Abilities)
			{
				if (InputComponent)
				{
					InputComponent->ClearInputBinding(AbilityHandle);
				}
				AbilitySystemComponent->SetRemoveAbilityOnEnd(AbilityHandle);
			}
		}

		ActiveExtensions.Remove(Actor);
	}
}


#undef LOCTEXT_NAMESPACE
