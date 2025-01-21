// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WrriorEnemyGameplayAbility.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Controllers/WarriorHeorController.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

AWarriorEnemyCharacter* UWrriorEnemyGameplayAbility::GetEnemyCharacterFromActroInfo()
{
    if (!CachedWarriorEnemyCharacter.IsValid())
    {
        CachedWarriorEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }
    return CachedWarriorEnemyCharacter.IsValid() ? CachedWarriorEnemyCharacter.Get() : nullptr;
}

UEnemyCombetComponent* UWrriorEnemyGameplayAbility::GetEnemyCombetComponentFromActroInfo()
{
    return GetEnemyCharacterFromActroInfo()->GetEnemyCombetComponent();
}

FGameplayEffectSpecHandle UWrriorEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
    check(EffectClass);

    FGameplayEffectContextHandle  ContextHandle = GetWrriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetWrriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);
    EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));

    return EffectSpecHandle;
}
