// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTT_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USBTT_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
//Functions
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
//Functions

//Variables
	UPROPERTY(EditAnywhere,Category="AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere,Category="AI")
	float MaxBulletSpread;

public:
	USBTT_RangedAttack();
};
