// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

void USAction::StartAction_Implementation(AActor* instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this))
}

void USAction::StopAction_Implementation(AActor* instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this))
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* comp = Cast<UActorComponent>(GetOuter());
	if (comp)
	{
		return comp->GetWorld();
	}
	return nullptr;
}
