// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WrriorGameplayAbility.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "Components/Combet/PawnCombetComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UWrriorGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EWrriorAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UWrriorGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (AbilityActivationPolicy == EWrriorAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombetComponent* UWrriorGameplayAbility::GetPawnCombetComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombetComponent>();
}

UWrriorAbilitySystemComponent* UWrriorGameplayAbility::GetWrriorAbilitySystemComponentFromActorInfo() const
{
	return Cast<UWrriorAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UWrriorGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent*ASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(ASC && InSpecHandle.IsValid());

	return GetWrriorAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, ASC);
}

FActiveGameplayEffectHandle UWrriorGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle  ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}
