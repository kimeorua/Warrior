// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WrriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbility_TargetLock : public UWrriorHeroGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	bool TryLockOnTarget();
	void GetAvailableActorsToLock();

	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	void GetAvaildableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	void InitTargetLockMovement();
	void InitTargetLockMappingContext();

	void CanacelTargetLockAbility();
	void CleanUp();
	void ResetTargetLockMovement();
	void ResetTargetLockMappingContext();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float BoxTraceDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TArray<TEnumAsByte<	EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	bool bShowPersistenDebugShape = false;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	TSubclassOf<UWarriorWidgetBase>TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockRotationInterpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockMaxWalkSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	UInputMappingContext* TargetLockMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockCameraOffsetDistance = 20.f;

	UPROPERTY()
	TArray<AActor*>AvailableActorsToLock;

	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetLockWidget;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	UPROPERTY()
	float CachedDefaultMaxWalkSpeed = 0.0f;
};
