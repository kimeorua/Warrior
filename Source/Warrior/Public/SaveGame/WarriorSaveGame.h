// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WrriorTypes/WarriorEnumTypes.h"
#include "WarriorSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	EWarriorGameDifficulty SaveCurrentGameDifficulty;
	
};
