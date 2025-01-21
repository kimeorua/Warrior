// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombetInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "WarriorBaseCharacter.generated.h"

class UWrriorAbilitySystemComponent;
class UWrroirAttributeSet;
class UDataAsset_StartUpDataBase;
class UMotionWarpingComponent;

UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombetInterface, public IPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarriorBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UPawnCombetComponent* GetPawnCombetComponent() const override;

	virtual UPawnUIComponent* GetPawnUIComponent() const override;

protected:

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	UWrriorAbilitySystemComponent* WrriorAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System")
	UWrroirAttributeSet* WrriorAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Data")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;


public:
	FORCEINLINE UWrriorAbilitySystemComponent* GetWrriorAbilitySystemComponent() const { return WrriorAbilitySystemComponent; }
	FORCEINLINE UWrroirAttributeSet* GetWrriorAttributeSet() const { return WrriorAttributeSet; }

};
