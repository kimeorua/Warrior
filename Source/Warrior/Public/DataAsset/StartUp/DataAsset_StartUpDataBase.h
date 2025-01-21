// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWrriorGameplayAbility;
class UWrriorAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUp Data")
	TArray<TSubclassOf<UWrriorGameplayAbility>>ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUp Data")
	TArray<TSubclassOf<UWrriorGameplayAbility>>ReactivateAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUp Data")
	TArray < TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UWrriorGameplayAbility>>& InAbilityToGive, UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

};
