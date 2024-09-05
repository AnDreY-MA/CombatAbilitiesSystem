// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DataAbilitySystemComponent.h"

#include "CombatAbilitiesSystemRuntimeModule.h"
#include "Abilities/CombatAbilityBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DataAbilitySystemComponent)

void UDataAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if(AbilityActorInfo)
	{
		if(AbilityActorInfo->AnimInstance == nullptr)
		{
			AbilityActorInfo->AnimInstance = AbilityActorInfo->GetAnimInstance();
		}

		if(UGameInstance* GameInstance{InOwnerActor->GetGameInstance()}; GameInstance)
		{
			GameInstance->GetOnPawnControllerChanged().AddDynamic(this, &UDataAbilitySystemComponent::OnPawnControllerChanged);
		}
	}

	GrantAbilitiesAndAttributes();
}

void UDataAbilitySystemComponent::BeginDestroy()
{
	if(AbilityActorInfo && AbilityActorInfo->OwnerActor.IsValid())
	{
		if(UGameInstance* GameInstance{AbilityActorInfo->OwnerActor->GetGameInstance()}; GameInstance)
		{
			GameInstance->GetOnPawnControllerChanged().RemoveAll(this);
		}
	}
	Super::BeginDestroy();
}

FGameplayAbilitySpecHandle UDataAbilitySystemComponent::GrantAbilityOfType(TSubclassOf<UGameplayAbility> InAbilityType,
	const bool bRemoveAfterActivation)
{
	FGameplayAbilitySpecHandle AbilitySpecHandle;
	if(InAbilityType)
	{
		FGameplayAbilitySpec AbilitySpec(InAbilityType);
		AbilitySpec.RemoveAfterActivation = bRemoveAfterActivation;

		AbilitySpecHandle = GiveAbility(AbilitySpec);
	}

	return AbilitySpecHandle;
}

void UDataAbilitySystemComponent::GrantAbilitiesAndAttributes()
{
	RemoveAllAbilitiesAndAttributes();

	SetupAbilities();

	SetupAttributes();
}

void UDataAbilitySystemComponent::OnPawnControllerChanged(APawn* InPawn, AController* InNewController)
{
	if(AbilityActorInfo && AbilityActorInfo->OwnerActor == InPawn && AbilityActorInfo->PlayerController != InNewController)
	{
		AbilityActorInfo->InitFromActor(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get(), this);
	}
}

void UDataAbilitySystemComponent::RemoveAllAbilitiesAndAttributes()
{
	for(UAttributeSet* AttributeSet : AddedAttributes)
	{
		RemoveSpawnedAttribute(AttributeSet);
	}
	for(FGameplayAbilitySpecHandle AbilitySpecHandle : DefaultAbilityHandle)
	{
		SetRemoveAbilityOnEnd(AbilitySpecHandle);
	}
	AddedAttributes.Empty(DefaultAttributes.Num());
	DefaultAbilityHandle.Empty(DefaultAbilities.Num());
}

bool UDataAbilitySystemComponent::IsUsingAbilityByClass(const TSubclassOf<UGameplayAbility> InAbilityClass) const
{
	if(!InAbilityClass)
	{
		UE_LOG(LogCombatAbilitySystem, Error, TEXT("IsUsingAbilityByClass() provided AbilityClass is null"))
		return false;
	}

	return GetActiveAbilitiesByClass(InAbilityClass).Num() > 0;
}

TArray<UGameplayAbility*> UDataAbilitySystemComponent::GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> InAbilitySearch) const
{
	TArray<FGameplayAbilitySpec> Specs = GetActivatableAbilities();
	TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
	TArray<UGameplayAbility*> ActiveAbilities;

	for(const FGameplayAbilitySpec& Spec : Specs)
	{
		if(Spec.Ability && Spec.Ability.GetClass()->IsChildOf(InAbilitySearch))
		{
			MatchingGameplayAbilities.Add(const_cast<FGameplayAbilitySpec*>(&Spec));
		}
	}

	for(const FGameplayAbilitySpec* Spec : MatchingGameplayAbilities)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();
		for(UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			if(ActiveAbility->IsActive())
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}

	return ActiveAbilities;
}


void UDataAbilitySystemComponent::SetupAbilities()
{
	DefaultAbilityHandle.Reserve(DefaultAbilities.Num());
	for(const TSubclassOf<UCombatAbilityBase>& Ability : DefaultAbilities)
	{
		if(*Ability)
		{
			DefaultAbilityHandle.Add(GiveAbility(FGameplayAbilitySpec(Ability)));
		}
	}
}

void UDataAbilitySystemComponent::SetupAttributes()
{
	for(const FCombatAttributeData& AttributeData : DefaultAttributes)
	{
		if(AttributeData.AttributeSetType)
		{
			UAttributeSet* NewAttributeSet {NewObject<UAttributeSet>(this, AttributeData.AttributeSetType)};
			if(AttributeData.DataTable)
			{
				NewAttributeSet->InitFromMetaDataTable(AttributeData.DataTable);
			}
			AddedAttributes.Add(NewAttributeSet);
			AddAttributeSetSubobject(NewAttributeSet);
		}
	}
}