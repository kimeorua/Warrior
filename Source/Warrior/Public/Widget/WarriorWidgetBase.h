// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"

class UHeroUIComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Hero UIComponent Initalized"))
	void BP_OnOwningHeroUIComponentInitalized(UHeroUIComponent* OwingHeroUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UIComponent Initalized"))
	void BP_OnOwningEnemyUIComponentInitalized(UEnemyUIComponent* OwingEnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreateWidget(AActor* OwingEnemyActor);
};
