// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorBaseCharacter.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "AbilitySystem/WrroirAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	WrriorAbilitySystemComponent = CreateDefaultSubobject<UWrriorAbilitySystemComponent>(TEXT("WrriorAbilitySystemComponent"));
	WrriorAttributeSet = CreateDefaultSubobject<UWrroirAttributeSet>(TEXT("WrriorAttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWrriorAbilitySystemComponent();
}

UPawnCombetComponent* AWarriorBaseCharacter::GetPawnCombetComponent() const
{
	return nullptr;
}

UPawnUIComponent* AWarriorBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WrriorAbilitySystemComponent)
	{
		WrriorAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgat to assign start up data to %s"), *GetName());
	}
}
