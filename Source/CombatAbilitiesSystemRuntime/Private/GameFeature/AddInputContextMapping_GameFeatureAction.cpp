// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeature/AddInputContextMapping_GameFeatureAction.h"
#include "CombatAbilitiesSystemRuntime/Public/CombatAbilitiesSystemRuntimeModule.h"
#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#define LOCTEXT_NAMESPACE "CombatAbilitiesSystemRuntimeFeatures"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AddInputContextMapping_GameFeatureAction)

void UAddInputContextMapping_GameFeatureAction::OnGameFeatureActivating()
{
	if (!ensure(ExtensionRequestHandles.IsEmpty()) ||
		!ensure(ControllersAddedTo.IsEmpty()))
	{
		Reset();
	}
	Super::OnGameFeatureActivating();
}

void UAddInputContextMapping_GameFeatureAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	Reset();
}

#if WITH_EDITORONLY_DATA
void UAddInputContextMapping_GameFeatureAction::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsValid())
	{
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, InputMapping.ToSoftObjectPath());
		AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, InputMapping.ToSoftObjectPath());
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UAddInputContextMapping_GameFeatureAction::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result {CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid)};

	if (InputMapping.IsNull())
	{
		Result = EDataValidationResult::Invalid;
		ValidationErrors.Add(LOCTEXT("NullInputMapping", "Null InputMapping."));
	}

	return Result;
}
#endif

void UAddInputContextMapping_GameFeatureAction::AddToWorld(const FWorldContext& WorldContext)
{
	const UWorld* World {WorldContext.World()};
	const UGameInstance* GameInstance {WorldContext.OwningGameInstance.Get()};

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentMan {UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance)})
		{
			if (!InputMapping.IsNull())
			{
				const UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate {UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
					this, &UAddInputContextMapping_GameFeatureAction::HandleControllerExtension)};
				const TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle {ComponentMan->AddExtensionHandler(APlayerController::StaticClass(), AddAbilitiesDelegate)};

				ExtensionRequestHandles.Add(ExtensionRequestHandle);
			}
		}
	}
}

void UAddInputContextMapping_GameFeatureAction::Reset()
{
	ExtensionRequestHandles.Empty();

	while (!ControllersAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APlayerController> ControllerPtr {ControllersAddedTo.Top()};
		if (ControllerPtr.IsValid())
		{
			RemoveInputMapping(ControllerPtr.Get());
		}
		else
		{
			ControllersAddedTo.Pop();
		}
	}
}

void UAddInputContextMapping_GameFeatureAction::HandleControllerExtension(AActor* InActor, FName InEventName)
{
	APlayerController* AsController {CastChecked<APlayerController>(InActor)};

	if (InEventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || InEventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveInputMapping(AsController);
	}
	else if (InEventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || InEventName == UGameFrameworkComponentManager::NAME_GameActorReady)
	{
		AddInputMappingForPlayer(AsController->GetLocalPlayer());
	}
}

void UAddInputContextMapping_GameFeatureAction::AddInputMappingForPlayer(UPlayer* InPlayer)
{
	if (const ULocalPlayer* LocalPlayer {Cast<ULocalPlayer>(InPlayer)})
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem {LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()})
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), Priority);
			}
		}
		else
		{
			UE_LOG(LogCombatAbilitySystem, Error, TEXT("Failed to find `UEnhancedInputLocalPlayerSubsystem` for local player. Input mappings will not be added. Make sure you're set to use the EnhancedInput system via config file."));
		}
	}
}

void UAddInputContextMapping_GameFeatureAction::RemoveInputMapping(APlayerController* InPlayerController)
{
	if (const ULocalPlayer* LocalPlayer {InPlayerController->GetLocalPlayer()})
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->RemoveMappingContext(InputMapping.Get());
		}
	}

	ControllersAddedTo.Remove(InPlayerController);
}

#undef LOCTEXT_NAMESPACE