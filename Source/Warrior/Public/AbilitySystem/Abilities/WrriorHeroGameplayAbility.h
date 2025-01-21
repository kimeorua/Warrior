// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WrriorGameplayAbility.h"
#include "WrriorHeroGameplayAbility.generated.h"

class AWarriorHeroCharacter;
class AWarriorHeorController;
class UHeroCombetComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWrriorHeroGameplayAbility : public UWrriorGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Wrroir | Ability")
	AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Wrroir | Ability")
	AWarriorHeorController* GetWarriorHeorControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Wrroir | Ability")
	UHeroCombetComponent* GetHeroCombetComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Wrroir | Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandel(TSubclassOf<UGameplayEffect> EffectClass, float  InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

private:
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AWarriorHeorController> CachedWarriorHeorController;
};
