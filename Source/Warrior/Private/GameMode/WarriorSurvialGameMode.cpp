// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/WarriorSurvialGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Engine/TargetPoint.h"
#include "WrriorFunctionLibrary.h"

#include "WarriorDebugHelper.h"

void AWarriorSurvialGameMode::BeginPlay()
{
	Super::BeginPlay();

	checkf(EnemyWaveSpawnerDataTable, TEXT("Forgot to assigne a vaild data table in survial game mode BP"));

	SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);

	TotalWaveToSpawn = EnemyWaveSpawnerDataTable->GetRowNames().Num();

	PreLoadNextWaveEnemies();
}

void AWarriorSurvialGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::WaitSpawnNewWave)
	{
		TimePassedSinceToStart += DeltaSeconds;

		if (TimePassedSinceToStart >= SpawnNewWaveWaitTime)
		{
			TimePassedSinceToStart = 0.f;

			SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::SpawningNewWave);
		}
	}

	if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::SpawningNewWave)
	{
		TimePassedSinceToStart += DeltaSeconds;

		if (TimePassedSinceToStart >= SpawnEnemiesDelayTime)
		{
			CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

			TimePassedSinceToStart = 0.f;
			SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::Inprogress);
		}
	}

	if (CurrentSurvialGameModeState == EWarriorSurvialGameModeState::WaveCompleted)
	{
		TimePassedSinceToStart += DeltaSeconds;
		if (TimePassedSinceToStart >= WaitCompletedWaitTime)
		{
			TimePassedSinceToStart = 0.0f;

			CurrentWaveCount++;

			if (HasFinishedAllWaves())
			{
				SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::AllWaveDone);
			}
			else
			{
				SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaitSpawnNewWave);
				PreLoadNextWaveEnemies();
			}
		}
	}
}

void AWarriorSurvialGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


	EWarriorGameDifficulty SaveGameDifficulty;

	if (UWrriorFunctionLibrary::TryLoadSaveGameDifficulty(SaveGameDifficulty))
	{
		CurrentGameDifficulty = SaveGameDifficulty;
	}
}

void AWarriorSurvialGameMode::SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState InState)
{
	CurrentSurvialGameModeState = InState;
	OnSurvialGameModeStateChanged.Broadcast(CurrentSurvialGameModeState);
}

bool AWarriorSurvialGameMode::HasFinishedAllWaves() const
{
	return CurrentWaveCount > TotalWaveToSpawn;
}

void AWarriorSurvialGameMode::PreLoadNextWaveEnemies()
{
	if (HasFinishedAllWaves()) { return; }

	PreLoadedEnemyClassMap.Empty();

	for (const FWarriorEnemyWaveSpawnerInfo SpawnerInfo : GetCurrentWaveSpawnerTableRaw()->EnemyWaveSpawnerDefinitions)
	{
		if (SpawnerInfo.SoftEnemyClassToSapwn.IsNull()) { continue; }

		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			SpawnerInfo.SoftEnemyClassToSapwn.ToSoftObjectPath(), 
			FStreamableDelegate::CreateLambda(
				[SpawnerInfo, this]()
				{
					if (UClass* LoadEnemyClass = SpawnerInfo.SoftEnemyClassToSapwn.Get())
					{
						PreLoadedEnemyClassMap.Emplace(SpawnerInfo.SoftEnemyClassToSapwn, LoadEnemyClass);
					}
				}
			)
		);
	}
}

int32 AWarriorSurvialGameMode::TrySpawnWaveEnemies()
{
	if (TargetPointsArray.IsEmpty())
	{
		UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPointsArray);
	}

	checkf(!TargetPointsArray.IsEmpty(), TEXT("No vaild TargetPoint Actor In Level %s"), *GetWorld()->GetName());

	uint32 EnemiesSpawnedThisTime = 0;

	FActorSpawnParameters SpawnParm;
	SpawnParm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FWarriorEnemyWaveSpawnerInfo SpanwerInfo : GetCurrentWaveSpawnerTableRaw()->EnemyWaveSpawnerDefinitions)
	{
		if (SpanwerInfo.SoftEnemyClassToSapwn.IsNull()) { continue; }

		const int32 NumToSpawn = FMath::RandRange(SpanwerInfo.MinPerSpawnCount, SpanwerInfo.MaxPerSpawnCount);

		UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(SpanwerInfo.SoftEnemyClassToSapwn);

		for (int32  i = 0; i < NumToSpawn; i++)
		{
			const int32 RandomTargetPointIndex = FMath::RandRange(0, TargetPointsArray.Num() - 1);
			FVector SpawnOrigin = TargetPointsArray[RandomTargetPointIndex]->GetActorLocation();
			FRotator SpawnRatation= TargetPointsArray[RandomTargetPointIndex]->GetActorForwardVector().ToOrientationRotator();

			FVector RandomLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, SpawnOrigin, RandomLocation, 400.f);

			RandomLocation += FVector(0.f, 0.f, 150.f);

			AWarriorEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AWarriorEnemyCharacter>(LoadedEnemyClass, RandomLocation, SpawnRatation, SpawnParm);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
				EnemiesSpawnedThisTime++;
				TotalSpawnedEnemiesThisWaveCounter++;
			}

			if (!ShouldKeepSpawnEnemies())
			{
				return EnemiesSpawnedThisTime;
			}
		}
	}
	return EnemiesSpawnedThisTime;
}

bool AWarriorSurvialGameMode::ShouldKeepSpawnEnemies() const
{
	return TotalSpawnedEnemiesThisWaveCounter < GetCurrentWaveSpawnerTableRaw()->TotalEnemyToSpawnThisWave;
}

void AWarriorSurvialGameMode::OnEnemyDestroyed(AActor* DestroyedActor)
{
	CurrentSpawnedEnemiesCounter--;

	Debug::Print(FString::Printf(TEXT("CurrentSpawnedEnemiesCounter: %i,  TotalSpawnedEnemiesThisWaveCounter: %i"), CurrentSpawnedEnemiesCounter, TotalSpawnedEnemiesThisWaveCounter));

	if (ShouldKeepSpawnEnemies())
	{
		CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
	}
	else if (CurrentSpawnedEnemiesCounter == 0)
	{
		TotalSpawnedEnemiesThisWaveCounter = 0;
		CurrentSpawnedEnemiesCounter = 0;

		SetCurrentSurvialGameModeState(EWarriorSurvialGameModeState::WaveCompleted);
	}
}

FWarriorEnemyWaveSpawnerTableRaw* AWarriorSurvialGameMode::GetCurrentWaveSpawnerTableRaw() const
{
	const FName RowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount));
	FWarriorEnemyWaveSpawnerTableRaw* FoundRow = EnemyWaveSpawnerDataTable->FindRow<FWarriorEnemyWaveSpawnerTableRaw>(RowName, FString());

	checkf(FoundRow, TEXT("Could not find a vaild row under the name %s in the data table"), *RowName.ToString());
	
	return FoundRow;
}

void AWarriorSurvialGameMode::RegisterSpawnedEnemies(const TArray<AWarriorEnemyCharacter*>& InEnemiesToRagister)
{
	for (AWarriorEnemyCharacter* SpawnedEnemy : InEnemiesToRagister)
	{
		if (SpawnedEnemy)
		{
			CurrentSpawnedEnemiesCounter++;

			SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this, &ThisClass::OnEnemyDestroyed);
		}
	}
}
