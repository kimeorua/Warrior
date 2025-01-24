// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickUp/WarriorStonepBase.h"
#include "Characters/WarriorHeroCharacter.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"

void AWarriorStonepBase::Consume(UWrriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);

	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, AbilitySystemComponent->MakeEffectContext());

	BP_OnStoneConsumed();
}

void AWarriorStonepBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWarriorHeroCharacter* OverlppedHeroCharacter = Cast<AWarriorHeroCharacter>(OtherActor))
	{
		OverlppedHeroCharacter->GetWrriorAbilitySystemComponent()->TryActivateAbilityByTag(WarriorGameplayTags::Player_Ability_PickUp_Stone);
	}
}
