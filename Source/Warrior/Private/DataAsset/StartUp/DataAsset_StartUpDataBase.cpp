// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUp/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WrriorGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactivateAbilities, InASCToGive, ApplyLevel);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InASCToGive->MakeEffectContext());
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWrriorGameplayAbility>>& InAbilityToGive, UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilityToGive.IsEmpty())
	{
		return;
	}
	for (const TSubclassOf<UWrriorGameplayAbility>& Ability : InAbilityToGive)
	{
		if (!Ability)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();

		InASCToGive->GiveAbility(AbilitySpec);

	}
}
