// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WrriorHeroGameplayAbility.h"
#include "Controllers/WarriorHeorController.h"
#include "Characters/WarriorHeroCharacter.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

AWarriorHeroCharacter* UWrriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorHeorController* UWrriorHeroGameplayAbility::GetWarriorHeorControllerFromActorInfo()
{
	if (!CachedWarriorHeorController.IsValid())
	{
		CachedWarriorHeorController = Cast<AWarriorHeorController>(CurrentActorInfo->PlayerController);
	}
	return CachedWarriorHeorController.IsValid() ? CachedWarriorHeorController.Get() : nullptr;
}


UHeroCombetComponent* UWrriorHeroGameplayAbility::GetHeroCombetComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombetComponent();
}

FGameplayEffectSpecHandle UWrriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandel(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	FGameplayEffectContextHandle  ContextHandle = GetWrriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());


	FGameplayEffectSpecHandle EffectSpecHandle = GetWrriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponBaseDamage);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
	}

	return EffectSpecHandle;
}

bool UWrriorHeroGameplayAbility::GetAbilityRemainingCooldawnByTag(FGameplayTag InCooldawnTag, float& TotalCooldawnTime, float& RemainingCooldawnTime)
{
	check(InCooldawnTag.IsValid());

	FGameplayEffectQuery CooldawnQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldawnTag.GetSingleTagContainer());

	TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldawnQuery);
	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldawnTime = TimeRemainingAndDuration[0].Key;
		TotalCooldawnTime = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldawnTime > 0.f;
}
