// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combet/PawnCombetComponent.h"
#include "HeroCombetComponent.generated.h"

class AWrriorHeroWeapon;

/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroCombetComponent : public UPawnCombetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Wrrior | Combet")
	AWrriorHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag);

	UFUNCTION(BlueprintPure, Category = "Wrrior | Combet")
	AWrriorHeroWeapon* GetHeroCurrentEqquipedWeapon();

	UFUNCTION(BlueprintPure, Category = "Wrrior | Combet")
	float GetHeroCurrentEqquipedWeaponDamageAtLevel(float InLevel);

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnPulledFromTargetActor(AActor* InteractedActor) override;

};
