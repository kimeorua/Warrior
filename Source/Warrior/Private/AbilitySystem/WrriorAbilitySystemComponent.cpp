// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WrriorHeroGameplayAbility.h"
#include "WarriorGameplayTags.h"

void UWrriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;
		
		if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		} 
	}
}

void UWrriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MuseBeHeld))
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UWrriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWrriorHeroAbilitySet>& InDefaultAbilities, const TArray<FWrriorHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantAbilitySpecHandles)
{
	if (InDefaultAbilities.IsEmpty())
	{
		return;
	}
	for (const FWrriorHeroAbilitySet& AbilitySet : InDefaultAbilities)
	{
		if (!AbilitySet.IsVaild()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	for (const FWrriorHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsVaild()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UWrriorAbilitySystemComponent::RemoveGrantHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandleToRemove)
{
	if (InSpecHandleToRemove.IsEmpty()) return;

	for (FGameplayAbilitySpecHandle& SpecHandle : InSpecHandleToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}
	InSpecHandleToRemove.Empty();
}

bool UWrriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;

	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() -1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}
	return false;
}
