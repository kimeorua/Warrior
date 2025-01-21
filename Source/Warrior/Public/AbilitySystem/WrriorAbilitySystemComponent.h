// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WrriorTypes/WrriorStructType.h"
#include "WrriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWrriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Wrrior | Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FWrriorHeroAbilitySet>& InDefaultAbilities, const TArray<FWrriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Wrrior | Ability")
	void RemoveGrantHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandleToRemove);

	UFUNCTION(BlueprintCallable, Category = "Wrrior | Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
