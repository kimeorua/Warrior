// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combet/PawnCombetComponent.h"
#include "Items/Weapons/WrriorWeaponBase.h"
#include "WarriorDebugHelper.h"
#include "Components/BoxComponent.h"

void UPawnCombetComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWrriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A named %s has already been added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnPulledFromTargetActor);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AWrriorWeaponBase* UPawnCombetComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (AWrriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AWrriorWeaponBase* UPawnCombetComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombetComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else
	{
		ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);
	}
	//TODO : Handle Body Collision Boxes

}

void UPawnCombetComponent::OnHitTargetActor(AActor* HitActor)
{

}

void UPawnCombetComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}

void UPawnCombetComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	AWrriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

	check(WeaponToToggle);

	if (bShouldEnable)
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlapActors.Empty();
	}
}

void UPawnCombetComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
}
