// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WrriorTypes/WarriorEnumTypes.h"
#include "WrriorGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EWrriorAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

class UPawnCombetComponent;
class UWrriorAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWrriorGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin APawn Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End APawn Interface

	UPROPERTY(EditDefaultsOnly, Category = "Wrroir Ability")
	EWrriorAbilityActivationPolicy AbilityActivationPolicy = EWrriorAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Wrroir | Ability")
	UPawnCombetComponent* GetPawnCombetComponentFromActorInfo()const;

	UFUNCTION(BlueprintPure, Category = "Wrroir | Ability")
	UWrriorAbilitySystemComponent* GetWrriorAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "Wrroir | Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Wrroir | Ability")
	void ApplyGameplayEffectSpecHandleToHitResult(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResult);

};
