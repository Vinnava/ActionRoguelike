// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponet.h"

#include "InputBehavior.h"
#include "SGameplayInterface.h"
#include "StaticMeshAttributes.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), true, TEXT("Enable debug Draw for interactions"), ECVF_Cheat);


// Sets default values for this component's properties
USInteractionComponet::USInteractionComponet()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponet::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponet::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInteractionComponet::PrimaryInteract()
{
	bool bDrawDebug = CVarDebugDrawInteraction.GetValueOnGameThread();
	//AActor* MyOwner = GetOwner();
	TObjectPtr<AActor> MyOwner = GetOwner();
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * 500);
	//FHitResult HitResult;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, EyeLocation, End, ObjectQueryParams);
	
	TArray<FHitResult> HitResults;

	float Radius = 30.0f;
	
	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	FColor SphereColor = bBlockingHit ? FColor::Red : FColor::Green;

	for (FHitResult HitResult : HitResults)
	{
		TObjectPtr<AActor> HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				TObjectPtr<APawn> MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);

				if (bDrawDebug) DrawDebugSphere(GetWorld(), HitResult.Location, Radius, 32, SphereColor, false, 2.0f);
				
				break;
			}
		}
	}
	
	//FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;
	//DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 5.0f, 0.0f, 1.0f);
	
}