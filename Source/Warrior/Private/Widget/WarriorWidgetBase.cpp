// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WarriorWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UHeroUIComponent* HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnOwningHeroUIComponentInitalized(HeroUIComponent);
		}
	}
}

void UWarriorWidgetBase::InitEnemyCreateWidget(AActor* OwingEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwingEnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent, TEXT("Faild to extrac an EnemyUIComponent from %s"), *OwingEnemyActor->GetActorNameOrLabel());

		BP_OnOwningEnemyUIComponentInitalized(EnemyUIComponent);
	}
}
