// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WrriorHeroWeapon.h"

void AWrriorHeroWeapon::AssignGrantAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWrriorHeroWeapon::GetGrantAbilitySpecHandle() const
{
	return GrantAbilitySpecHandles;
}
