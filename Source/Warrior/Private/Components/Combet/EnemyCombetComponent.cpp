// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combet/EnemyCombetComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "WarriorGameplayTags.h"
#include "WrriorFunctionLibrary.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Components/BoxComponent.h"

#include "WarriorDebugHelper.h"

void UEnemyCombetComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlapActors.Contains(HitActor))
	{
		return;
	}
	OverlapActors.Add(HitActor);

	bool bIsVaildBlock = false;

	const bool bIsPlayerBlocking = UWrriorFunctionLibrary::NativeDoseActorHaveTag(HitActor, WarriorGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = UWrriorFunctionLibrary::NativeDoseActorHaveTag(GetOwningPawn(), WarriorGameplayTags::Enemy_Status_UnBlockable);

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsVaildBlock = UWrriorFunctionLibrary::IsVaildBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsVaildBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, WarriorGameplayTags::Player_Event_SuccessfulBlock, EventData);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), WarriorGameplayTags::Shared_Event_MeleeHit, EventData);
	}
}

void UEnemyCombetComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AWarriorEnemyCharacter* OwningEnemyCharacter = GetOwningPawn<AWarriorEnemyCharacter>();
	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetLeftHandCollisionBox();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetRightHandCollisionBox();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}

	if (!bShouldEnable)
	{
		OverlapActors.Empty();
	}
}
