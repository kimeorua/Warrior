// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WrriorWeaponBase.generated.h"

class UBoxComponent;

 DECLARE_DELEGATE_OneParam(FOnTargetInteractableDelegate, AActor*)

UCLASS()
class WARRIOR_API AWrriorWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWrriorWeaponBase();

	FOnTargetInteractableDelegate OnWeaponHitTarget;
	FOnTargetInteractableDelegate OnWeaponPulledFromTarget;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

	UFUNCTION()
	virtual void OnCollisionBoxBegineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	
	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const { return WeaponCollisionBox; }
};
