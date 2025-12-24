// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> defaultAction : defaultActions)
	{
		AddAction(defaultAction);
	}
	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USActionComponent::AddAction(TSubclassOf<USAction> actionClass)
{
	if (!actionClass) return;
	
	USAction* newAction = NewObject<USAction>(this, actionClass);
	if (newAction)
	{
		actions.Add(newAction);
	}
}

bool USActionComponent::StartActionByName(AActor* instigator, FName actionName)
{
	for (USAction* action : actions)
	{
		if (action->actionName == actionName)
		{
			action->StartAction(instigator);
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* instigator, FName actionName)
{
	for (USAction* action : actions)
	{
		if (action->actionName == actionName)
		{
			action->StopAction(instigator);
			return true;
		}
	}
	return false;
}

