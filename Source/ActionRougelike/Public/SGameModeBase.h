// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private://Variables
	
	
protected://Variables
	FTimerHandle timerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category= AI)
	float spawnTimerInterval {0.2f};
	
	UPROPERTY(EditDefaultsOnly, Category= AI)
	UEnvQuery* spawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category= AI)
	TSubclassOf<AActor> minionClass;

	UPROPERTY(EditDefaultsOnly, Category= AI)
	UCurveFloat* difficultyCurve;
	
	// Read/write access as we could change this as our difficulty increases via Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 creditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	UEnvQuery* powerupSpawnQuery;

	// All power-up classes used to spawn with EQS at match start
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> powerupClasses;

	// Distance required between power-up spawn locations 
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float requiredPowerupDistance;

	// Amount of powerups to spawn during match start
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 desiredPowerupCount;

public://Variables
	
	
private://Functions
	

protected://Functions
	UFUNCTION()
	void OnBotSpawnQueryCompleted(class UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type queryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* queryInstance, EEnvQueryStatus::Type queryStatus);

	UFUNCTION()
	void SpawnTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* controller);
	
public://Functions
	ASGameModeBase();
	
	virtual void StartPlay() override;
	
	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* victimActor, AActor* killer);

};
