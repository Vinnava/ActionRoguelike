// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float Damage)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -Damage);
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
			// Direction = Target - Origin
			FVector HitDirection = OutHit.TraceEnd - OutHit.TraceStart;
			
			HitComp->AddImpulseAtLocation(HitDirection * 300000.f, OutHit.ImpactPoint, OutHit.BoneName);
		}
		return true;
	}
	
	return false;
}
