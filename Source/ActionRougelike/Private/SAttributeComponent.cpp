// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
	
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	//UEngine::AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(FString("Health: %f"), Health));

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
	
}


