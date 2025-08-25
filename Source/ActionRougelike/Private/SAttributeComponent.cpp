// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	
	//UEngine::AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(FString("Health: %f"), Health));

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	
	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	const USAttributeComponent* AttributeComponent = GetAttributes(FromActor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	return false;
}

