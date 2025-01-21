// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h"
#include "WrroirAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IPawnUIInterface;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWrroirAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWrroirAttributeSet();
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, CurrentRage)

	UPROPERTY(BlueprintReadOnly, Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, MaxRage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefencePower;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, DefencePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UWrroirAttributeSet, DamageTaken)

private:
	TWeakInterfacePtr<IPawnUIInterface>CachedPawnUIInterface;
};
