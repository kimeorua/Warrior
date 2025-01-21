// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombetComponent.generated.h"

class AWrriorWeaponBase;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UPawnCombetComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Wrrior | Combet")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWrriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Wrrior | Combet")
	AWrriorWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UFUNCTION(BlueprintCallable, Category = "Wrrior | Combet")
	AWrriorWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UPROPERTY(BlueprintReadWrite, Category = "Wrrior | Combet")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Wrrior | Combet")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnPulledFromTargetActor(AActor* InteractedActor);

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

	TArray<AActor*>OverlapActors;

private:
	TMap<FGameplayTag, AWrriorWeaponBase*>CharacterCarriedWeaponMap;
};
