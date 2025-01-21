#pragma once
#include "Engine.h"

// 디버그용 출력 함수 정의

namespace Debug
{
	static void Print(const FString& msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 Inkey = -1, const FColor& Color = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(":  ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(Inkey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}