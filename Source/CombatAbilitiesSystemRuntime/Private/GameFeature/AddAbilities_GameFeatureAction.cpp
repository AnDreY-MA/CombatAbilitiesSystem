
#include "GameFeature/AddAbilities_GameFeatureAction.h"

#include "AbilitySystemComponent.h"
#include "CombatAbilitiesSystemRuntimeModule.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Components/CombatSystemComponent.h"
#include "Engine/AssetManager.h"
#include "Input/AbilityInputBindingComponent.h"


#define LOCTEXT_NAMESPACE "CombatAbilitiesSystemFeatures"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AddAbilities_GameFeatureAction)


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
	if (!UAssetManager::IsValid()) return;
	
	auto AddBundleAsset = [&AssetBundleData](const FTopLevelAssetPath& SoftObjectPath)
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, SoftObjectPath);
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, SoftObjectPath);
	};

	for (const FGameFeatureAbilitiesEntry& Entry : AbilitiesList)
	{
		for (const FCombatAbilityMapping& Ability : Entry.GrantedAbilities)
		{
			AddBundleAsset(FTopLevelAssetPath(Ability.Ability->GetPathName()));
			if (!Ability.InputAction.IsNull())
			{
				AddBundleAsset(FTopLevelAssetPath{Ability.InputAction->GetPathName()});
			}
		}

		for (const FCombatAttributesMapping& Attributes : Entry.GrantedAttributes)
		{
			AddBundleAsset(FTopLevelAssetPath(Attributes.Attribute->GetPathName()));
			if (!Attributes.AttributeData.IsNull())
			{
				AddBundleAsset(FTopLevelAssetPath(Attributes.AttributeData->GetPathName()));
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UAddAbilities_GameFeatureAction::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result {CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid)};

	int32 EntryIndex {0};
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

		int32 AbilityIndex {0};
		for (const FCombatAbilityMapping& Ability : Entry.GrantedAbilities)
		{
			if (Ability.Ability.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				ValidationErrors.Add(FText::Format(LOCTEXT("EntryHasNullAbility", "Null AbilityType at index {0} in AbilitiesList[{1}].GrantedAbilities"), FText::AsNumber(AbilityIndex), FText::AsNumber(EntryIndex)));
			}
			++AbilityIndex;
		}

		int32 AttributesIndex {0};
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
	const UWorld* World {WorldContext.World()};
	const UGameInstance* GameInstance {WorldContext.OwningGameInstance};
	if(!GameInstance && !World && !World->IsGameWorld()) return;

	UGameFrameworkComponentManager* ComponentManager {UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance)};
	if(!ComponentManager)
	{
		UE_LOG(LogCombatAbilitySystem, Warning, TEXT("Failed to get UGameFrameworkComponentManager from %s"), *GameInstance->GetName())
		return;
	}

	int32 EntryIndex {0};
	for (const FGameFeatureAbilitiesEntry& Entry : AbilitiesList)
	{
		if (!Entry.ActorClass.IsNull())
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate {UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
				this, &UAddAbilities_GameFeatureAction::HandleActorExtension, EntryIndex)};
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle {ComponentManager->AddExtensionHandler(Entry.ActorClass, AddAbilitiesDelegate)};

			ComponentRequests.Add(ExtensionRequestHandle);
			EntryIndex++;
		}
	}
	
}

void UAddAbilities_GameFeatureAction::Reset()
{
	while (!ActiveExtensions.IsEmpty())
	{
		const auto ExtensionIt = ActiveExtensions.CreateIterator();
		RemoveActorAbilities(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UAddAbilities_GameFeatureAction::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (AbilitiesList.IsValidIndex(EntryIndex))
	{
		const FGameFeatureAbilitiesEntry& Entry {AbilitiesList[EntryIndex]};
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
	auto* CombatAbilitySystemComponent {FindOrAddComponentForActor<UCombatSystemComponent>(Actor, AbilitiesEntry)};
	if(!CombatAbilitySystemComponent)
	{
		UE_LOG(LogCombatAbilitySystem, Error, TEXT("Failed to find/add an ability component to '%s'. Abilities will not be granted."), *Actor->GetPathName());
		return;
	}

	FActorExtensions AddedExtensions;
	AddedExtensions.Abilities.Reserve(AbilitiesEntry.GrantedAbilities.Num());
	AddedExtensions.Attributes.Reserve(AbilitiesEntry.GrantedAttributes.Num());

	for (const FCombatAbilityMapping& Ability : AbilitiesEntry.GrantedAbilities)
	{
		if (Ability.Ability.IsNull()) continue;

		FGameplayAbilitySpec NewAbilitySpec(Ability.Ability.LoadSynchronous());
		FGameplayAbilitySpecHandle AbilityHandle = CombatAbilitySystemComponent->GiveAbility(NewAbilitySpec);

		if (!Ability.InputAction.IsNull())
		{
			if (auto* InputComponent = FindOrAddComponentForActor<UAbilityInputBindingComponent>(Actor, AbilitiesEntry); InputComponent)
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

	for (const FCombatAttributesMapping& Attributes : AbilitiesEntry.GrantedAttributes)
	{
		if (Attributes.Attribute.IsNull()) continue;

		if (TSubclassOf<UAttributeSet> SetType = Attributes.Attribute.LoadSynchronous(); SetType)
		{
			UAttributeSet* NewSet = NewObject<UAttributeSet>(CombatAbilitySystemComponent, SetType);
			if (!Attributes.AttributeData.IsNull())
			{
				if (UDataTable* InitData = Attributes.AttributeData.LoadSynchronous(); InitData)
				{
					NewSet->InitFromMetaDataTable(InitData);
				}
			}

			AddedExtensions.Attributes.Add(NewSet);
			CombatAbilitySystemComponent->AddAttributeSetSubobject(NewSet);
		}
	}

	ActiveExtensions.Add(Actor, AddedExtensions);
	
}

UActorComponent* UAddAbilities_GameFeatureAction::FindOrAddComponentForActor(UClass* InComponentType,
	const AActor* InActor, const FGameFeatureAbilitiesEntry& InAbilitiesEntry)
{
	UActorComponent* Component {InActor->FindComponentByClass(InComponentType)};
	
	bool bMakeComponentRequest {Component == nullptr};
	if (Component)
	{
		if (Component->CreationMethod == EComponentCreationMethod::Native)
		{
			const UObject* ComponentArchetype = Component->GetArchetype();
			bMakeComponentRequest = ComponentArchetype->HasAnyFlags(RF_ClassDefaultObject);
		}
	}

	if (bMakeComponentRequest)
	{
		const UWorld* World = InActor->GetWorld();
		const UGameInstance* GameInstance = World->GetGameInstance();

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
	FActorExtensions* ActorExtensions {ActiveExtensions.Find(Actor)};
	if(!ActorExtensions) return;

	if (auto* CombatAbilitySystemComponent {Actor->FindComponentByClass<UCombatSystemComponent>()})
	{
		for (UAttributeSet* AttribSetInstance : ActorExtensions->Attributes)
		{
			CombatAbilitySystemComponent->GetSpawnedAttributes_Mutable().Remove(AttribSetInstance);
		}

		auto* InputComponent {Actor->FindComponentByClass<UAbilityInputBindingComponent>()};
		for (FGameplayAbilitySpecHandle AbilityHandle : ActorExtensions->Abilities)
		{
			if (InputComponent)
			{
				InputComponent->ClearInputBinding(AbilityHandle);
			}
			CombatAbilitySystemComponent->SetRemoveAbilityOnEnd(AbilityHandle);
		}
	}

	ActiveExtensions.Remove(Actor);
	
}

#undef LOCTEXT_NAMESPACE