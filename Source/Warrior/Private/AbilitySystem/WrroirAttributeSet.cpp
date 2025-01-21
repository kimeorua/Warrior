// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WrroirAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WrriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "WarriorDebugHelper.h"

UWrroirAttributeSet::UWrroirAttributeSet()
{
	InitCurrentHealth(1.0f);
	InitMaxHealth(1.0f);
	InitCurrentRage(1.0f);
	InitMaxRage(1.0f);
	InitAttackPower(1.0f);
	InitDefencePower(1.0f);
}

void UWrroirAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}

	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorLabel());
	
	UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

	checkf(PawnUIComponent, TEXT("Couldn't extra a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorLabel());

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.0f, GetMaxRage());

		SetCurrentRage(NewCurrentRage);

		if (GetCurrentRage() == GetMaxRage())
		{
			UWrriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full);
		}
		else if (GetCurrentRage() == 0.f)
		{
			UWrriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None);
		}
		else
		{
			UWrriorFunctionLibrary::RemoveGameplayTagToActorIfFind(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_Full); 
			UWrriorFunctionLibrary::RemoveGameplayTagToActorIfFind(Data.Target.GetAvatarActor(), WarriorGameplayTags::Player_Status_Rage_None); 
		}

		if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
		{
			HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurentHealth = FMath::Clamp(OldHealth - DamageDone, 0.0f, GetMaxHealth());

		SetCurrentHealth(NewCurentHealth);

		//const FString DebugString = FString::Printf(TEXT("OldHealth : %f, DamageDone : %f, NewCurentHealth : %f"), OldHealth, DamageDone, NewCurentHealth);

		//Debug::Print(DebugString, FColor::Green);

		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		if (GetCurrentHealth() == 0.0f)
		{
			UWrriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Death);
		}
	}
}
