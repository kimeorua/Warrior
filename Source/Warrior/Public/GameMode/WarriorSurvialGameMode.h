// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/WarriorBaseGameMode.h"
#include "WarriorSurvialGameMode.generated.h"

class AWarriorEnemyCharacter;

UENUM(BlueprintType)
enum class EWarriorSurvialGameModeState : uint8
{
	WaitSpawnNewWave,
	SpawningNewWave,
	Inprogress,
	WaveCompleted,
	AllWaveDone,
	PlayerDied
};

USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AWarriorEnemyCharacter> SoftEnemyClassToSapwn;

	UPROPERTY(EditAnywhere)
	int32 MinPerSpawnCount = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxPerSpawnCount = 3;
};

USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRaw : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> EnemyWaveSpawnerDefinitions;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyToSpawnThisWave = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvialGameModeStateChangedDelegate, EWarriorSurvialGameModeState, CurrentState);

/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorSurvialGameMode : public AWarriorBaseGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
private:
	void SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState InState);
	bool HasFinishedAllWaves() const;
	void PreLoadNextWaveEnemies();
	int32 TrySpawnWaveEnemies();
	bool ShouldKeepSpawnEnemies() const;

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	FWarriorEnemyWaveSpawnerTableRaw* GetCurrentWaveSpawnerTableRaw() const;

	UPROPERTY()
	EWarriorSurvialGameModeState CurrentSurvialGameModeState;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnSurvialGameModeStateChangedDelegate OnSurvialGameModeStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyWaveSpawnerDataTable;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 TotalWaveToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveCount = 1;

	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;

	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;

	UPROPERTY()
	TArray<AActor*>TargetPointsArray;

	UPROPERTY()
	float TimePassedSinceToStart = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnNewWaveWaitTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float SpawnEnemiesDelayTime = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WaveDefinition", meta = (AllowPrivateAccess = "true"))
	float WaitCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap<TSoftClassPtr<AWarriorEnemyCharacter>, UClass*> PreLoadedEnemyClassMap;

public:
	UFUNCTION(BlueprintCallable)
	void RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRagister);
};
