// Fill out your copyright notice in the Description page of Project Settings.


#include "WrriorTypes/WrriorStructType.h"
#include "AbilitySystem/Abilities/WrriorHeroGameplayAbility.h"

bool FWrriorHeroAbilitySet::IsVaild() const
{
    return InputTag.IsValid() && AbilityToGrant;
}
