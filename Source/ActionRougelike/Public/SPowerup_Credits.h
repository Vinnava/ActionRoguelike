// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPowerup_Credits : public ASPowerupActor
{
	GENERATED_BODY()

private://Variables
	
	
protected://Variables
	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 creditsAmount;

public://Variables
	
	
private://Functions
	

protected://Functions
	
	
public://Functions
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_Credits();
	
};
