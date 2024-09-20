// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Components/PawnComponent.h"
#include "PlayerControlsComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputMappingContext;


UCLASS(ClassGroup="CombatAbilitiesSystem", BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class COMBATABILITIESSYSTEM_API UPlayerControlsComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

protected:
	UFUNCTION(BlueprintNativeEvent, Category="Player Controls")
	void SetupPlayerControls(UEnhancedInputComponent* InPlayerInputComponent);
	
	UFUNCTION(BlueprintNativeEvent, Category="Player Controls")
	void TeardownPlayerControls(UEnhancedInputComponent* InPlayerInputComponent);

	template<class UserClass, typename FuncType>
	bool BindInputAction(const UInputAction* InAction, const ETriggerEvent InEventType, UserClass* InObject, FuncType InFunction)
	{
		if(ensure(InputComponent != nullptr) && ensure(InAction != nullptr))
		{
			InputComponent->BindAction(InAction, InEventType, InObject, InFunction);
			return true;
		}
		return false;
	}

	UFUNCTION()
	virtual void OnPawnRestarted(APawn* InPawn);
	UFUNCTION()
	virtual void OnControllerChanged(APawn* InPawn, AController* InOldController, AController* NewController);

	virtual void SetupInputComponent(APawn* InPawn);
	virtual void ReleaseInputComponent(AController* OldController = nullptr);
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(AController* OldController = nullptr) const;
	
	UEnhancedInputComponent* GetInputComponent() const { return InputComponent;}

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Controls", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> InputMappingContext{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Controls", meta=(AllowPrivateAccess="true"))
	int32 InputPriority{0};

	UPROPERTY(Transient)
	UEnhancedInputComponent* InputComponent;
	
};