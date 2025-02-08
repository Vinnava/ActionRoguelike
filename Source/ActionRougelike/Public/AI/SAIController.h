// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"


class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASAIController : public AAIController
{
	GENERATED_BODY()
	
//Functions
	virtual void BeginPlay() override;


//Variables	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
};
