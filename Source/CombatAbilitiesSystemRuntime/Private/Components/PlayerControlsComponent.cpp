// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerControlsComponent.h"

#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlayerControlsComponent)

void UPlayerControlsComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* InPlayerInputComponent)
{
}

void UPlayerControlsComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* InPlayerInputComponent)
{
}

void UPlayerControlsComponent::OnRegister()
{
	Super::OnRegister();

	const UWorld* World {GetWorld()};
	APawn* PawnOwner{GetPawn<APawn>()};
	if(!ensure(PawnOwner) && !World->IsGameWorld()) return;

	PawnOwner->ReceiveRestartedDelegate.AddDynamic(this, &UPlayerControlsComponent::OnPawnRestarted);
	PawnOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &UPlayerControlsComponent::OnControllerChanged);

	if(PawnOwner->InputComponent)
	{
		OnPawnRestarted(PawnOwner);
	}
	
}

void UPlayerControlsComponent::OnUnregister()
{
	if(const UWorld* World {GetWorld()}; World && World->IsGameWorld())
	{
		ReleaseInputComponent();
		if(auto* PawnOwner{GetPawn<APawn>()}; PawnOwner)
		{
			PawnOwner->ReceiveRestartedDelegate.RemoveAll(this);
			PawnOwner->ReceiveControllerChangedDelegate.RemoveAll(this);
		}
	}
	
	Super::OnUnregister();
}

void UPlayerControlsComponent::OnPawnRestarted(APawn* InPawn)
{
	if(ensure(InPawn && InPawn == GetOwner()) && InPawn->InputComponent)
	{
		ReleaseInputComponent();
		if(InPawn->InputComponent)
		{
			SetupInputComponent(InPawn);
		}
	}
}

void UPlayerControlsComponent::OnControllerChanged(APawn* InPawn, AController* InOldController,
	AController* NewController)
{
	if(ensure(InPawn && InPawn == GetOwner()) && InOldController)
	{
		ReleaseInputComponent(InOldController);
	}
}

void UPlayerControlsComponent::SetupInputComponent(APawn* InPawn)
{
	InputComponent = CastChecked<UEnhancedInputComponent>(InPawn->InputComponent);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = {GetEnhancedInputSubsystem()};
	check(Subsystem);
	if(InputMappingContext)
	{
		Subsystem->AddMappingContext(InputMappingContext, InputPriority);
	}
	SetupPlayerControls(InputComponent);
}

void UPlayerControlsComponent::ReleaseInputComponent(AController* OldController)
{
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem {GetEnhancedInputSubsystem(OldController)}; Subsystem && InputComponent)
	{
		TeardownPlayerControls(InputComponent);
		if(InputMappingContext)
		{
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
	InputComponent = nullptr;
}

UEnhancedInputLocalPlayerSubsystem* UPlayerControlsComponent::GetEnhancedInputSubsystem(
	AController* OldController) const
{
	const APlayerController* PlayerController {GetController<APlayerController>()};
	if(!PlayerController)
	{
		PlayerController = Cast<APlayerController>(OldController);
		if(!PlayerController)
		{
			return nullptr;
		}
	}

	const ULocalPlayer* LocalPlayer {PlayerController->GetLocalPlayer()};
	if(!LocalPlayer) return nullptr;

	return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}
