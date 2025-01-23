// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "WrriorTypes/WarriorEnumTypes.h"

class FWarriorCountdownAction : public FPendingLatentAction
{
public:
	FWarriorCountdownAction(float InTotalCountDawnTime, float InUpdateInterval, float& OutRemainingTime, EWarriorCountawnActionOutput& InCountdownOutput, const FLatentActionInfo& LatentInfo) 
		: 
		bNeedToCancel(false),
		TotalCountdownTime(InTotalCountDawnTime),
		UpdateInterval(InUpdateInterval),
		OutRemainingTime(OutRemainingTime),
		CountdownOutput(InCountdownOutput),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget),
		ElapsedInterval(0.0f),
		ElapsedTimeSinceStart(0.0f)
	{
	}

	void CancelAction();

	virtual void UpdateOperation(FLatentResponse& Response) override;

private:
	bool bNeedToCancel;
	float TotalCountdownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EWarriorCountawnActionOutput& CountdownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
