// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WrriorWeaponBase.h"
#include "WrriorTypes/WrriorStructType.h"
#include "GameplayAbilitySpecHandle.h"
#include "WrriorHeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWrriorHeroWeapon : public AWrriorWeaponBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Data")
	FWrriorHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle>GetGrantAbilitySpecHandle() const;

private:
	TArray<FGameplayAbilitySpecHandle>GrantAbilitySpecHandles;

};
