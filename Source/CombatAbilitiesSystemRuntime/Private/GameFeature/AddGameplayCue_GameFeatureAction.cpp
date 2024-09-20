// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/AddGameplayCue_GameFeatureAction.h"

#include "Misc/DataValidation.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AddGameplayCue_GameFeatureAction)

#define LOCTEXT_NAMESPACE "AddGameplayCue"

UAddGameplayCue_GameFeatureAction::UAddGameplayCue_GameFeatureAction(const FObjectInitializer& InInitializer) :
	Super(InInitializer)
{
	DirectoryPaths.Add(FDirectoryPath{TEXT("/GameplayCue")});
	
}

#if WITH_EDITOR
EDataValidationResult UAddGameplayCue_GameFeatureAction::IsDataValid(FDataValidationContext& Context)
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	for (const FDirectoryPath& Directory : DirectoryPaths)
	{
		if(Directory.Path.IsEmpty())
		{
			const FText InvalidCuePathError{ FText::Format(LOCTEXT("InvalidCuePathError", "'{0}' is not a valid path!"), FText::FromString(Directory.Path))};
			Context.AddError(InvalidCuePathError);
			Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
		}
	}
	
	return CombineDataValidationResults(Result, EDataValidationResult::Valid);
}
#endif

#undef LOCTEXT_NAMESPACE