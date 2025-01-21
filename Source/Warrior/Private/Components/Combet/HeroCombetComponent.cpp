// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combet/HeroCombetComponent.h"
#include "Items/Weapons/WrriorHeroWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"

AWrriorHeroWeapon* UHeroCombetComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag)
{
    return Cast<AWrriorHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AWrriorHeroWeapon* UHeroCombetComponent::GetHeroCurrentEqquipedWeapon()
{
    return Cast<AWrriorHeroWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UHeroCombetComponent::GetHeroCurrentEqquipedWeaponDamageAtLevel(float InLevel)
{
    return GetHeroCurrentEqquipedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UHeroCombetComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlapActors.Contains(HitActor))
    {
        return;
    }
    OverlapActors.AddUnique(HitActor);

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Shared_Event_MeleeHit, Data);

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UHeroCombetComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Player_Event_HitPause, FGameplayEventData());
}
