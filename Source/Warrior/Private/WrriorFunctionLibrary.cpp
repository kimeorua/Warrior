// Fill out your copyright notice in the Description page of Project Settings.


#include "WrriorFunctionLibrary.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h" 
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorGameplayTags.h"

#include "WarriorDebugHelper.h"

UWrriorAbilitySystemComponent* UWrriorFunctionLibrary::NativeGetWrroirASCFromActor(AActor* InActor)
{
	check(InActor);
	return CastChecked<UWrriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWrriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UWrriorAbilitySystemComponent* ASC = NativeGetWrroirASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWrriorFunctionLibrary::RemoveGameplayTagToActorIfFind(AActor* InActor, FGameplayTag TagToRemove)
{
	UWrriorAbilitySystemComponent* ASC = NativeGetWrroirASCFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWrriorFunctionLibrary::NativeDoseActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UWrriorAbilitySystemComponent* ASC = NativeGetWrroirASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWrriorFunctionLibrary::BP_DoseActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoseActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombetComponent* UWrriorFunctionLibrary::NativeGetPawnCombetComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IPawnCombetInterface* PawnCombetInterface = Cast<IPawnCombetInterface>(InActor))
	{
		return PawnCombetInterface->GetPawnCombetComponent();
	}
	return nullptr;
}

UPawnCombetComponent* UWrriorFunctionLibrary::BP_GetPawnCombetComponentFromActor(AActor* InActor, EWarriorVaildType& OutVaildType)
{
	UPawnCombetComponent* CombetComponent = NativeGetPawnCombetComponentFromActor(InActor);
	OutVaildType = CombetComponent ? EWarriorVaildType::Vaild : EWarriorVaildType::Invaild;
	return CombetComponent;
}

bool UWrriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}

float UWrriorFunctionLibrary::GetScalableFloatValueAtLeavel(const FScalableFloat& InScalableFloat, float InLeveal)
{
	return InScalableFloat.GetValueAtLevel(InLeveal);
}

FGameplayTag UWrriorFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
	check(InAttacker && InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalize = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

	const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalize);
	OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult); 

	const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalize);

	if (CrossResult.Z < 0.0f)
	{
		OutAngleDifference *= -1.0f;
	}

	if (OutAngleDifference >= -45.0f && OutAngleDifference <= 45.0f)
	{
		return WarriorGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference < -45.0f && OutAngleDifference >= -135.0f)
	{
		return WarriorGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference < -135.0f || OutAngleDifference >= 135.0f)
	{
		return WarriorGameplayTags::Shared_Status_HitReact_Back;
	}
	else if (OutAngleDifference > 45.0f && OutAngleDifference <= 135.0f)
	{
		return WarriorGameplayTags::Shared_Status_HitReact_Right;
	}
	return WarriorGameplayTags::Shared_Status_HitReact_Front;
}

bool UWrriorFunctionLibrary::IsVaildBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);
	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());

	return DotResult < -0.1f;
}

bool UWrriorFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstager, AActor* InTargetActor, const FGameplayEffectSpecHandle InSpecHandle)
{
	UWrriorAbilitySystemComponent* SourceASC = NativeGetWrroirASCFromActor(InInstager);
	UWrriorAbilitySystemComponent* TargetASC = NativeGetWrroirASCFromActor(InTargetActor);

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle  = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}
