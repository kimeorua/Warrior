// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUp/DataAsset_HeroStartUpData.h"
#include "AbilitySystem/Abilities/WrriorHeroGameplayAbility.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"

void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

    for (const FWrriorHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
    {
        if (!AbilitySet.IsVaild())continue;

        FGameplayAbilitySpec AbilitySpce(AbilitySet.AbilityToGrant);
        AbilitySpce.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpce.Level = ApplyLevel;
        AbilitySpce.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        InASCToGive->GiveAbility(AbilitySpce);
    }
}
