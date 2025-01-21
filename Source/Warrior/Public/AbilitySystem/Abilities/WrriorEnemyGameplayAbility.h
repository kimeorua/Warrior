// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WrriorGameplayAbility.h"
#include "WrriorEnemyGameplayAbility.generated.h"

class AWarriorEnemyCharacter;
class UEnemyCombetComponent;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWrriorEnemyGameplayAbility : public UWrriorGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior | Ability")
	AWarriorEnemyCharacter* GetEnemyCharacterFromActroInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior | Ability")
	UEnemyCombetComponent* GetEnemyCombetComponentFromActroInfo();

	UFUNCTION(BlueprintPure, Category = "Warrior | Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
	TWeakObjectPtr<AWarriorEnemyCharacter>CachedWarriorEnemyCharacter;
	
};
