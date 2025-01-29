#pragma once

UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EWarriorVaildType : uint8
{
	Vaild,
	Invaild
};

UENUM()
enum class EWarriorSuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EWarriorCountdawnActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EWarriorCountawnActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EWarriorGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EWarriorInputMode : uint8
{
	GameOnly,
	UIOnly
};