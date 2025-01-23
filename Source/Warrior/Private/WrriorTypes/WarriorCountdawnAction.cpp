// Fill out your copyright notice in the Description page of Project Settings.


#include "WrriorTypes/WarriorCountdawnAction.h"

void FWarriorCountdownAction::CancelAction()
{
	bNeedToCancel = true;
}

void FWarriorCountdownAction::UpdateOperation(FLatentResponse& Response)
{
	if (bNeedToCancel)
	{
		CountdownOutput = EWarriorCountawnActionOutput::Cancelled;
		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}
	
	if (ElapsedTimeSinceStart >= TotalCountdownTime)
	{
		CountdownOutput = EWarriorCountawnActionOutput::Completed;

		Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);

		return;
	}

	if (ElapsedInterval < UpdateInterval)
	{
		ElapsedInterval += Response.ElapsedTime();
	}
	else
	{
		ElapsedTimeSinceStart += UpdateInterval > 0.f ? UpdateInterval : Response.ElapsedTime();

		OutRemainingTime = TotalCountdownTime - ElapsedTimeSinceStart;

		CountdownOutput = EWarriorCountawnActionOutput::Updated;

		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);

		ElapsedInterval = 0.0f;
	}
}
