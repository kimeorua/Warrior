// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/StartUp/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UWrriorEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UWrriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUp Data")
	TArray<TSubclassOf<UWrriorEnemyGameplayAbility>>EnemyCombetAbility;

};
