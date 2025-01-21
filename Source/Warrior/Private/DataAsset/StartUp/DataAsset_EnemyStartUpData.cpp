// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/StartUp/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WrriorEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!EnemyCombetAbility.IsEmpty())
	{
		for (const TSubclassOf<UWrriorEnemyGameplayAbility>& AbilityClass : EnemyCombetAbility)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
