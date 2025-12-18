// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, playerState, int32, newCredits, int32, delta);

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected://Variables
	UPROPERTY(EditDefaultsOnly, Category= Credits)
	int32 credits;

public://Variables
	UPROPERTY(BlueprintAssignable, Category= Event)
	FOnCreditsChanged onCreditsChanged;
	
public://Functions
	UFUNCTION(BlueprintCallable, Category = Credits)
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = Credits)
	void AddCredits(int32 delta);

	UFUNCTION(BlueprintCallable, Category = Credits)
	bool RemoveCredits(int32 delta);
};
