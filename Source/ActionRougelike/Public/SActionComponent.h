// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()
private://Variables
	
	
protected://Variables
	UPROPERTY(EditAnywhere, Category = "Initalize|Actions")
	TArray<TSubclassOf<class USAction> > defaultActions;
	
	UPROPERTY()
	TArray<USAction*> actions;

public://Variables
	
	
private://Functions
	

protected://Functions
	// Called when the game starts
	virtual void BeginPlay() override;
	
public://Functions
	// Sets default values for this component's properties
	USActionComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AddAction(TSubclassOf<USAction> actionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* instigator, FName actionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* instigator, FName actionName);
};
