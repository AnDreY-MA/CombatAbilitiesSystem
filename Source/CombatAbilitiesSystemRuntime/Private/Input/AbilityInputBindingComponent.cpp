// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AbilityInputBindingComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityInputBindingComponent)

namespace AbilityInputBindingComponent_Impl
{
	constexpr int32 InvalidInputID{0};
	int32 IncrementingInputID{InvalidInputID};
	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}

void UAbilityInputBindingComponent::SetInputBinding(UInputAction* InInputAction,
	FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	FGameplayAbilitySpec* BindingAbility {FindAbilitySpec(AbilitySpecHandle)};

	FAbilityInputBinding* AbilityInputBinding {MappedAbilities.Find(InInputAction)};
	if(AbilityInputBinding)
	{
		if(FGameplayAbilitySpec* OldBoundAbility {FindAbilitySpec(AbilityInputBinding->BoundAbilitiesStack.Top())}; OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID)
		{
			OldBoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
		}
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InInputAction);
		AbilityInputBinding->InputID = AbilityInputBindingComponent_Impl::GetNextInputID();
	}

	if(BindingAbility)
	{
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	AbilityInputBinding->BoundAbilitiesStack.Push(AbilitySpecHandle);
	TryBindAbilityInput(InInputAction, *AbilityInputBinding);
	
}

void UAbilityInputBindingComponent::ClearInputBinding(FGameplayAbilitySpecHandle InAbilityHandle)
{
	FGameplayAbilitySpec* FoundAbility {FindAbilitySpec(InAbilityHandle)};
	if(!FoundAbility) return;

	auto MappedIterator = MappedAbilities.CreateIterator();
	while (MappedIterator)
	{
		if(MappedIterator.Value().InputID == FoundAbility->InputID)
		{
			break;
		}
		++MappedIterator;
	}

	if(!MappedIterator) return;

	FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();
	if(AbilityInputBinding.BoundAbilitiesStack.Remove(InAbilityHandle) > 0)
	{
		if(AbilityInputBinding.BoundAbilitiesStack.Num() > 0)
		{
			FGameplayAbilitySpec* StackedAbility {FindAbilitySpec(AbilityInputBinding.BoundAbilitiesStack.Top())};
			if(StackedAbility && StackedAbility->InputID == 0)
			{
				StackedAbility->InputID = AbilityInputBinding.InputID;
			}
		}
		else
		{
			RemoveEntry(MappedIterator.Key());
		}

		FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
	}

}

void UAbilityInputBindingComponent::ClearAbilityBindings(UInputAction* InInputAction)
{
	RemoveEntry(InInputAction);
	
}

void UAbilityInputBindingComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* InPlayerInputComponent)
{
	for(auto& InputBinding : MappedAbilities)
	{
		if(auto* CurrentInputComponent {GetInputComponent()}; CurrentInputComponent)
		{
			CurrentInputComponent->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			CurrentInputComponent->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}
		if(AbilityComponent)
		{
			const int32 ExpectedInputID = InputBinding.Value.InputID;
			for(FGameplayAbilitySpecHandle AbilityHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				if(FGameplayAbilitySpec* FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle); FoundAbility && FoundAbility->InputID == ExpectedInputID)
				{
					FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
				}
			}
		}
	}

	AbilityComponent = nullptr;
	
}

void UAbilityInputBindingComponent::ReleaseInputComponent(AController* OldController)
{
	ResetBindings();
	
	Super::ReleaseInputComponent(OldController);
	
}

void UAbilityInputBindingComponent::ResetBindings()
{
	for(auto& InputBinding : MappedAbilities)
	{
		if(auto* CurrentInputComponent {GetInputComponent()}; CurrentInputComponent)
		{
			CurrentInputComponent->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			CurrentInputComponent->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}
		if(AbilityComponent)
		{
			const int32 ExpectedInputID = InputBinding.Value.InputID;
			for(FGameplayAbilitySpecHandle AbilitySpecHandle : InputBinding.Value.BoundAbilitiesStack)
			{
				FGameplayAbilitySpec* FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilitySpecHandle);
				if(FoundAbility && FoundAbility->InputID == ExpectedInputID)
				{
					FoundAbility->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
				}
			}
		}

	}

	AbilityComponent = nullptr;
	
}

void UAbilityInputBindingComponent::RunAbilitySystemSetup()
{
	const AActor* Owner {GetOwner()};
	check(Owner);

	AbilityComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	if(!AbilityComponent) return;

	for (auto& InputBinding : MappedAbilities)
	{
		const int32 NewInputID {AbilityInputBindingComponent_Impl::GetNextInputID()};
		InputBinding.Value.InputID = NewInputID;
		for(const FGameplayAbilitySpecHandle AbilityHandle : InputBinding.Value.BoundAbilitiesStack)
		{
			if(FGameplayAbilitySpec* FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(AbilityHandle); FoundAbility)
			{
				FoundAbility->InputID = NewInputID;
			}
		}
	}
	
}

void UAbilityInputBindingComponent::OnAbilityInputPressed(UInputAction* InInputAction)
{
	if(!AbilityComponent)
	{
		RunAbilitySystemSetup();
	}
	if(AbilityComponent)
	{
		if(const FAbilityInputBinding* FoundBinding {MappedAbilities.Find(InInputAction)}; FoundBinding && ensure(FoundBinding->InputID) != AbilityInputBindingComponent_Impl::InvalidInputID)
		{
			AbilityComponent->AbilityLocalInputPressed(FoundBinding->InputID);
		}
	}
	
}

void UAbilityInputBindingComponent::OnAbilityInputReleased(UInputAction* InInputAction)
{
	if(!AbilityComponent) return;

	if(const FAbilityInputBinding* FoundBinding {MappedAbilities.Find(InInputAction)}; FoundBinding && ensure(FoundBinding->InputID != AbilityInputBindingComponent_Impl::InvalidInputID))
	{
		AbilityComponent->AbilityLocalInputReleased(FoundBinding->InputID);
	}
	
}

void UAbilityInputBindingComponent::RemoveEntry(UInputAction* InInputAction)
{
	if(FAbilityInputBinding* Binding {MappedAbilities.Find(InInputAction)})
	{
		if(auto* CurrentInputComponent {GetInputComponent()}; CurrentInputComponent)
		{
			CurrentInputComponent->RemoveBindingByHandle(Binding->OnPressedHandle);
			CurrentInputComponent->RemoveBindingByHandle(Binding->OnReleasedHandle);
		}

		for(FGameplayAbilitySpecHandle AbilityHandle : Binding->BoundAbilitiesStack)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpec(AbilityHandle);
			if(AbilitySpec && AbilitySpec->InputID == Binding->InputID)
			{
				AbilitySpec->InputID = AbilityInputBindingComponent_Impl::InvalidInputID;
			}
		}

		MappedAbilities.Remove(InInputAction);
	}
	
}

FGameplayAbilitySpec* UAbilityInputBindingComponent::FindAbilitySpec(FGameplayAbilitySpecHandle InHandle)
{
	FGameplayAbilitySpec* FoundAbility{nullptr};
	if(AbilityComponent)
	{
		FoundAbility = AbilityComponent->FindAbilitySpecFromHandle(InHandle);
	}
	return FoundAbility;
	
}

void UAbilityInputBindingComponent::TryBindAbilityInput(UInputAction* InInputAction,
	FAbilityInputBinding& InAbilityInputBinding)
{
	if(auto* CurrentInputComponent {GetInputComponent()}; CurrentInputComponent)
	{
		if(InAbilityInputBinding.OnPressedHandle == 0)
		{
			InAbilityInputBinding.OnPressedHandle = CurrentInputComponent->BindAction(InInputAction, ETriggerEvent::Started, this, &UAbilityInputBindingComponent::OnAbilityInputPressed, InInputAction).GetHandle();
		}
		
		if(InAbilityInputBinding.OnReleasedHandle == 0)
		{
			InAbilityInputBinding.OnReleasedHandle = CurrentInputComponent->BindAction(InInputAction, ETriggerEvent::Completed, this, &UAbilityInputBindingComponent::OnAbilityInputReleased, InInputAction).GetHandle();
		}
	}

}