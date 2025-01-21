// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WarriorCharacterAnimInstance.generated.h"

class AWarriorBaseCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds)override;

protected:
	UPROPERTY()
	AWarriorBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Data | LocomationData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Data | LocomationData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Data | LocomationData")
	float LocomationDirection;
};
