// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROUGELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
private://Variables
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> projectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName handSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float attackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* attackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* castingEffect;
	
protected://Variables
	

public://Variables
	
	
private://Functions
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* instigatorCharacter); 

protected://Functions
	
	
public://Functions
	USAction_ProjectileAttack();
	
	virtual void StartAction_Implementation(AActor* instigator) override;
};
