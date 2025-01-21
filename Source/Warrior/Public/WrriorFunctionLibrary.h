// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WrriorTypes/WarriorEnumTypes.h"
#include "WrriorFunctionLibrary.generated.h"


class UWrriorAbilitySystemComponent;
class UPawnCombetComponent;
struct FScalableFloat;

/**
 * 
 */
UCLASS()
class WARRIOR_API UWrriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UWrriorAbilitySystemComponent* NativeGetWrroirASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior | Function Library")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Warrior | Function Library")
	static void RemoveGameplayTagToActorIfFind(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoseActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Warrior | Function Library", meta = (DisplayName = "Dose Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoseActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);

	static UPawnCombetComponent* NativeGetPawnCombetComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior | Function Library", meta = (DisplayName = "Get Pawn Combet Component From Actor", ExpandEnumAsExecs = "OutVaildType"))
	static UPawnCombetComponent* BP_GetPawnCombetComponentFromActor(AActor* InActor, EWarriorVaildType& OutVaildType);

	UFUNCTION(BlueprintPure, Category = "Warrior | Function Library")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Warrior | Function Library", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLeavel(const FScalableFloat& InScalableFloat, float InLeveal = 1.0f);

	UFUNCTION(BlueprintPure, Category = "Warrior | Function Library")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference);

	UFUNCTION(BlueprintPure, Category = "Warrior | Function Library")
	static bool IsVaildBlock(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintCallable, Category = "Warrior | Function Library")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstager, AActor* InTargetActor, const FGameplayEffectSpecHandle InSpecHandle);
};
