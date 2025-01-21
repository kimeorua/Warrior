// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "WrriorStructType.generated.h"

class UWarriorHeroLinkedAnimLayer;
class UWrriorHeroGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWrriorHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWrriorHeroGameplayAbility> AbilityToGrant;

	bool IsVaild()const;
};

USTRUCT(BlueprintType)
struct FWrriorHeroSpecialAbilitySet : public FWrriorHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface>AbilityIconMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Player.Cooldawn"))
	FGameplayTag AbilityCooldownTag;
};
	

USTRUCT(BlueprintType)
struct FWrriorHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroLinkedAnimLayer> WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWrriorHeroAbilitySet>DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWrriorHeroSpecialAbilitySet>SpecialWeaponAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D>SoftWeaponIconTexture;
};
