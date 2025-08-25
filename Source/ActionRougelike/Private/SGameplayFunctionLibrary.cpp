// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float Damage)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		AttributeComp->ApplyHealthChange(DamageCauser, -Damage);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float Damage,
	const FHitResult& OutHit)
{
	if (ApplyDamage(DamageCauser, TargetActor, Damage))
	{
		UPrimitiveComponent* HitComp = OutHit.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(OutHit.BoneName))
		{
			HitComp->AddImpulseAtLocation(-OutHit.ImpactNormal * 300000.f, OutHit.ImpactPoint, OutHit.BoneName);
		}
		return true;
	}
	return false;
}
