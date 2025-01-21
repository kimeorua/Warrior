// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combet/PawnCombetComponent.h"
#include "EnemyCombetComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UEnemyCombetComponent : public UPawnCombetComponent
{
	GENERATED_BODY()
	
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;

protected:
	virtual void ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType) override;
};
