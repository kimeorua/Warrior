// Fill out your copyright notice in the Description page of Project Settings.


#include "WrriorFunctionLibrary.h"
#include "AbilitySystem/WrriorAbilitySystemComponent.h" 
#include "AbilitySystemBlueprintLibrary.h"
#include "Interfaces/PawnCombetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorGameplayTags.h"
#include "WrriorTypes/WarriorCountdawnAction.h"
#include "WarriorGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/WarriorSaveGame.h"

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

void UWrriorFunctionLibrary::CountDawn(const UObject* WorldContextObject, float TotalTime, float Updateinterval, float& OutRemaningTime, EWarriorCountdawnActionInput CountDawnInput, UPARAM(DisplayName = "Output") EWarriorCountawnActionOutput& CountDawnOutput, FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;
	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	if (!World)
	{
		return;
	}
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	FWarriorCountdownAction* FoundAction = LatentActionManager.FindExistingAction<FWarriorCountdownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	if (CountDawnInput == EWarriorCountdawnActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FWarriorCountdownAction(TotalTime, Updateinterval, OutRemaningTime, CountDawnOutput, LatentInfo));
		}
	}

	if (CountDawnInput == EWarriorCountdawnActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

UWarriorGameInstance* UWrriorFunctionLibrary::GetWarriorGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

		if (World)
		{
			return World->GetGameInstance<UWarriorGameInstance>();
		}
	}
	return nullptr;
}

void UWrriorFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EWarriorInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

		if (World)
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}
	if (!PlayerController) { return; }

	FInputModeGameOnly GameOnlyMode;
	FInputModeUIOnly UIOnly;

	switch (InInputMode)
	{

	case EWarriorInputMode::GameOnly: 
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;

		break;
	case EWarriorInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnly);
		PlayerController->bShowMouseCursor = true;
		break;
	default:
		break;
	}
}

void UWrriorFunctionLibrary::SaveCurrentGameDifficulty(EWarriorGameDifficulty InDifficultyToSave)
{
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UWarriorSaveGame::StaticClass());

	if (UWarriorSaveGame* WarriorSaveGmaeObject = Cast<UWarriorSaveGame>(SaveGameObject))
	{
		WarriorSaveGmaeObject->SaveCurrentGameDifficulty = InDifficultyToSave;

		const bool bWasSave = UGameplayStatics::SaveGameToSlot(WarriorSaveGmaeObject, WarriorGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		Debug::Print(bWasSave ? TEXT("Save") : TEXT("Not Save"));
	}
}

bool UWrriorFunctionLibrary::TryLoadSaveGameDifficulty(EWarriorGameDifficulty& OutDifficultyToSave)
{
	if (UGameplayStatics::DoesSaveGameExist(WarriorGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
	{
		USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(WarriorGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);

		if (UWarriorSaveGame* WarriorSaveGmaeObject = Cast<UWarriorSaveGame>(SaveGameObject))
		{
			OutDifficultyToSave = WarriorSaveGmaeObject->SaveCurrentGameDifficulty;

			Debug::Print(TEXT("Loading Successful"));

			return true;
		}
	}
	return false;
}
