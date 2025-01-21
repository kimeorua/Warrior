// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/WarriorWidgetBase.h"
#include "Controllers/WarriorHeorController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "WrriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "WarriorDebugHelper.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TryLockOnTarget())
	{
		InitTargetLockMovement();
		InitTargetLockMappingContext();
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor || 
		UWrriorFunctionLibrary::NativeDoseActorHaveTag(CurrentLockedActor, WarriorGameplayTags::Shared_Status_Death) ||
		UWrriorFunctionLibrary::NativeDoseActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Shared_Status_Death))
	{
		CanacelTargetLockAbility();
		return;
	}
	SetTargetLockWidgetPosition(); 

	const bool bShouldOverrideRotation =
		!UWrriorFunctionLibrary::NativeDoseActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Rolling)
		&&
		!UWrriorFunctionLibrary::NativeDoseActorHaveTag(GetHeroCharacterFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation
		(GetHeroCharacterFromActorInfo()->GetActorLocation(), CurrentLockedActor->GetActorLocation());

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.0f, 0.0f);

		const FRotator CurrentControllRot = GetWarriorHeorControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControllRot, LookAtRot, DeltaTime, TargetLockRotationInterpSpeed);

		GetWarriorHeorControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.0f));
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.0f, TargetRot.Yaw, 0.0f));
	}

}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*>ActorsOnLeft;
	TArray<AActor*>ActorsOnRight;

	AActor* NewTargetToLock = nullptr;

	GetAvaildableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

bool UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CanacelTargetLockAbility();
		return false;
	}
	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();

		SetTargetLockWidgetPosition(); 

		return true;
	}
	else
	{
		CanacelTargetLockAbility(); 

		return false;
	}
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();

	TArray<FHitResult>BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistenDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
	);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.0f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::GetAvaildableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CanacelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormailze = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvaildableActor : AvailableActorsToLock)
	{
		if (!AvaildableActor || AvaildableActor == CurrentLockedActor) continue;
		
		const FVector PlayerToAvaildableNormailze = (AvaildableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormailze, PlayerToAvaildableNormailze);

		if (CrossResult.Z > 0.0f)
		{
			OutActorsOnRight.AddUnique(AvaildableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvaildableActor);
		}
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to assgine a vaild widget class in Blueprint"));

		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetWarriorHeorControllerFromActorInfo(), TargetLockWidgetClass);

		check(DrawnTargetLockWidget);
		DrawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		CanacelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition;

	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetWarriorHeorControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{

		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	ScreenPosition -= (TargetLockWidgetSize / 2.f);

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;
	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetWarriorHeorControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(TargetLockMappingContext, 3); 
}

void UHeroGameplayAbility_TargetLock::CanacelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}

	DrawnTargetLockWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;
	
	CachedDefaultMaxWalkSpeed = 0.0f;
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.0f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UHeroGameplayAbility_TargetLock::ResetTargetLockMappingContext()
{
	if (!GetWarriorHeorControllerFromActorInfo()) { return; }

	const ULocalPlayer* LocalPlayer = GetWarriorHeorControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}
