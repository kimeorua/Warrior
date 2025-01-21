// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExeCalc/GEExeCalc_DamageTaken.h"
#include "AbilitySystem/WrroirAttributeSet.h"
#include "WarriorGameplayTags.h"
#include "WarriorDebugHelper.h"

struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefencePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);

	FWarriorDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWrroirAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWrroirAttributeSet, DefencePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWrroirAttributeSet, DamageTaken, Target, false);
	}
};

static const FWarriorDamageCapture& GetFWarriorDamageCapture()
{
	static FWarriorDamageCapture WarriorDamageCapture;

	return WarriorDamageCapture;
}

UGEExeCalc_DamageTaken::UGEExeCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetFWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetFWarriorDamageCapture().DefencePowerDef);
	RelevantAttributesToCapture.Add(GetFWarriorDamageCapture().DamageTakenDef);
}

void UGEExeCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec&  EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetFWarriorDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float BaseDamage = 0.0f;
	int32 UseLightAttackComboCount = 0;
	int32 UseHeavyAttackComboCount = 0;

	for (const TPair<FGameplayTag, float> TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage= TagMagnitude.Value;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UseLightAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UseLightAttackComboCount"), UseLightAttackComboCount);
		}
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UseHeavyAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UseHeavyAttackComboCount"), UseHeavyAttackComboCount);
		}
	}

	float TargetDefencePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetFWarriorDamageCapture().DefencePowerDef, EvaluateParameters, TargetDefencePower);
	//Debug::Print(TEXT("TargetDefencePower"), TargetDefencePower);


	if (UseLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UseLightAttackComboCount - 1) * 0.05f + 1.0f;
		BaseDamage *= DamageIncreasePercentLight;

		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}

	if (UseHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = UseHeavyAttackComboCount * 0.15f + 1.0f;
		BaseDamage *= DamageIncreasePercentHeavy;

		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefencePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetFWarriorDamageCapture().DamageTakenProperty, EGameplayModOp::Override, FinalDamageDone));
	}
}