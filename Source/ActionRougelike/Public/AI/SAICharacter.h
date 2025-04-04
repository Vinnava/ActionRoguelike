// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROUGELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	virtual void PostInitializeComponents() override;
	
//Functions
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
	
//Variables
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;
	

};
