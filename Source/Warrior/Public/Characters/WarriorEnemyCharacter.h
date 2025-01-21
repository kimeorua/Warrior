// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorEnemyCharacter.generated.h"

class UEnemyCombetComponent;
class UEnemyUIComponent;
class UWidgetComponent;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorEnemyCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorEnemyCharacter();

	virtual UPawnCombetComponent* GetPawnCombetComponent() const override;

	virtual UPawnUIComponent* GetPawnUIComponent() const override;

	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;

protected:
	virtual void BeginPlay() override;

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combet")
	UEnemyCombetComponent* EnemyCombetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combet")
	UBoxComponent* LeftHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combet")
	FName LeftHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combet")
	UBoxComponent* RightHandCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combet")
	FName RightHandCollisionBoxAttachBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

	UFUNCTION()
	virtual void OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombetComponent* GetEnemyCombetComponent() const {return EnemyCombetComponent; }

	FORCEINLINE UBoxComponent* GetLeftHandCollisionBox() const { return LeftHandCollisionBox; }
	FORCEINLINE UBoxComponent* GetRightHandCollisionBox() const { return RightHandCollisionBox; }
};
