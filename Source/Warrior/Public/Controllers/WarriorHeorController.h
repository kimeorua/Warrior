// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "WarriorHeorController.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeorController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AWarriorHeorController();

	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	FGenericTeamId HeroTeamId;
};
