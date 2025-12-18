// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	//Default Values
	creditsPerKill = 20;

	desiredPowerupCount = 10;
	requiredPowerupDistance = 2000;

	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn determined by spawn logic later in the chain.
	GetWorldTimerManager().SetTimer(timerHandle_SpawnBots, this, &ASGameModeBase::SpawnTimerElapsed, spawnTimerInterval, true);

	// Make sure we have assigned at least one power-up class
	if (ensure(powerupClasses.Num() > 0))
	{
		// Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, powerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* bot = *It;

		USAttributeComponent* attributeComp = USAttributeComponent::GetAttributes(bot);
		if (attributeComp && attributeComp->IsAlive())
		{
			attributeComp->Kill(this); // @fixme: Pass in Player? for kill credits
		}
	}
}

void ASGameModeBase::SpawnTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		return;
	}
	int32 noOfBotsAlive = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		const USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && AttributeComp->IsAlive())
		{
			noOfBotsAlive++;
		}
	}
	
	int32 maxBotCount = 10;
	
	if (difficultyCurve)
	{
		maxBotCount = difficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
    
	if (noOfBotsAlive >= maxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* queryInstance = UEnvQueryManager::RunEQSQuery(this,
		spawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(queryInstance))
	{
		queryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnActorKilled(AActor* victimActor, AActor* killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(victimActor), *GetNameSafe(killer));

	// Respawn Players after delay
	ASCharacter* player = Cast<ASCharacter>(victimActor);
	if (player)
	{
		FTimerHandle timerHandle_RespawnDelay;

		FTimerDelegate delegate;
		delegate.BindUObject(this, &ASGameModeBase::RespawnPlayerElapsed, player->GetController());

		float respawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(timerHandle_RespawnDelay, delegate, respawnDelay, false);
	}

	// Give Credits for kill
	APawn* killerPawn = Cast<APawn>(killer);
	if (killerPawn)
	{
		if (ASPlayerState* PS = killerPawn->GetPlayerState<ASPlayerState>()) // < can cast and check for nullptr within if-statement.
		{
			PS->AddCredits(creditsPerKill);
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* controller)
{
	if (controller)
	{
		controller->UnPossess();
		RestartPlayer(controller);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type queryStatus)
{
	if (queryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed"))
		return;
	}

	TArray<FVector> Locations = queryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(minionClass, Locations[0], FRotator::ZeroRotator);

		// Draw Debug Sphere
		DrawDebugSphere(GetWorld(), Locations[0], 15.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* queryInstance,
	EEnvQueryStatus::Type queryStatus)
{
	if (queryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> locations = queryInstance->GetResultsAsLocations();

	// Keep used locations to easily check distance between points
	TArray<FVector> usedLocations;

	int32 spawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (spawnCounter < desiredPowerupCount && locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 randomLocationIndex = FMath::RandRange(0, locations.Num() - 1);

		FVector pickedLocation = locations[randomLocationIndex];
		// Remove to avoid picking again
		locations.RemoveAt(randomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector otherLocation : usedLocations)
		{
			float DistanceTo = (pickedLocation - otherLocation).Size();

			if (DistanceTo < requiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation) continue;

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, powerupClasses.Num() - 1);
		TSubclassOf<AActor> randomPowerupClass = powerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(randomPowerupClass, pickedLocation, FRotator::ZeroRotator);
		UE_LOG(LogTemp, Log, TEXT("[Test] randomPowerupClass %s"), *GetNameSafe(randomPowerupClass));

		// Keep for distance checks
		usedLocations.Add(pickedLocation);
		spawnCounter++;
	}
}
