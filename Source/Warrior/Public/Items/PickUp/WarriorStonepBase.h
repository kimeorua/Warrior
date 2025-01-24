// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUp/WarriorPckUpBase.h"
#include "WarriorStonepBase.generated.h"

class UWrriorAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorStonepBase : public AWarriorPckUpBase
{
	GENERATED_BODY()

public:
	void Consume(UWrriorAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Stone Consumed"))
	void BP_OnStoneConsumed();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StoneGameplayEffectClass;

};
