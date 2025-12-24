// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class ACTIONROUGELIKE_API USAction : public UObject
{
	GENERATED_BODY()
	
private://Variables
	
	
protected://Variables
	

public://Variables
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName actionName;
	
private://Functions
	

protected://Functions
	
	
public://Functions
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* instigator);
	
	virtual class UWorld* GetWorld() const override;
};
