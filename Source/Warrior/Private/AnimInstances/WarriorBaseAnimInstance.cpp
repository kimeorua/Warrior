// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WrriorFunctionLibrary.h"

bool UWarriorBaseAnimInstance::DoseOwnerHasTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UWrriorFunctionLibrary::NativeDoseActorHaveTag(OwningPawn, TagToCheck);
	}
	return false;
}
