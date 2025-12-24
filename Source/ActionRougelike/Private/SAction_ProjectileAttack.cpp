// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	attackAnimDelay = 0.2f;
	handSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* instigator)
{
	Super::StartAction_Implementation(instigator);
	
	ACharacter* character = Cast<ACharacter>(instigator);
	if (character)
	{
		character->PlayAnimMontage(attackAnim);
	
		UGameplayStatics::SpawnEmitterAttached(castingEffect, character->GetMesh(), handSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, attackAnimDelay, false);
		
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* instigatorCharacter)
{
	if (ensure(projectileClass))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("AttackDelay_Elapsed")));
		
		FVector handLocation = instigatorCharacter->GetMesh()->GetSocketLocation(handSocketName);

		/*
		//(Previously used Start&End)Not using this because the start location is the exact camera location. So, it's colliding with wall
		
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);
		*/
		
		FVector startOffset;
		
		ASCharacter* playerCharacter = Cast<ASCharacter>(instigatorCharacter);
		if (playerCharacter)
		{
			startOffset = playerCharacter->GetCameraComponent()->GetForwardVector() * playerCharacter->GetSpringArmComponent()->TargetArmLength;
		}

		FVector TraceStart = instigatorCharacter->GetPawnViewLocation() + startOffset;// + (CameraComp->GetForwardVector() * SpringArmComp->TargetArmLength);
		FVector TraceEnd = TraceStart + (instigatorCharacter->GetControlRotation().Vector() * 5000);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		
		FHitResult Hit;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(instigatorCharacter);

		
		
		/*//Debug Line
		bool LineHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectQueryParams);
		FColor LineColor = LineHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 20.0f, 0.0f, 1.0f);
		
		DrawDebugSphere(GetWorld(), TraceStart, 10.0f, 32, FColor::Blue, false, 20.0f);
		DrawDebugSphere(GetWorld(), TraceEnd, 10.0f, 32, FColor::Blue, false, 20.0f);*/

		
		FTransform SpawnTM;
		FRotator SpawnRot;
		
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, Params))
		{
			SpawnRot = UKismetMathLibrary::FindLookAtRotation(handLocation, Hit.Location);
			
			DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 32, FColor::Red, false, 2.0f);
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("HitActor : %s"), *Hit.GetActor()->GetName()));
		}
		else
		{
			SpawnRot = UKismetMathLibrary::FindLookAtRotation(handLocation, Hit.TraceEnd);
		}

		SpawnTM = FTransform(SpawnRot, handLocation);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = instigatorCharacter;
                
		GetWorld()->SpawnActor<AActor>(projectileClass, SpawnTM, SpawnParams);

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Spawn Class : %s"), *ClassToSpawn->GetName()));
	}
	
	StopAction(instigatorCharacter);
}