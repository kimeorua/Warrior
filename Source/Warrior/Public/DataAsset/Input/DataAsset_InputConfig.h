// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

/// <summary>
/// Input용 구조체  설정
/// </summary>
USTRUCT(BlueprintType)
struct  FWarriorInputActionConfig
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction = nullptr;

	bool IsVaild() const
	{
		return InputTag.IsValid() && InputAction;
	}
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorInputActionConfig> AbilityInputActions;

	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InputTag) const;

};
