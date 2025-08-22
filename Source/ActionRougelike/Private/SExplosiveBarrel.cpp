// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(StaticMesh);
	RadialForce->Radius = 600.0f;
	RadialForce->ImpulseStrength = 2000.0f;
	RadialForce->bImpulseVelChange = true;

}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Ensure the OtherActor is valid
	if (ensure(OtherActor) && OtherActor != GetInstigator()) // Prevent self-reference
	{
		RadialForce->FireImpulse();

		FString CombinedString = FString::Printf(TEXT("Hit Location: %s"), *Hit.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Blue, 5.0f, false);

		FString OtherActorName = OtherActor->GetName();
		GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Purple,FString::Printf(TEXT("OtherActor Name: %s"), *OtherActorName));
		
		USAttributeComponent* AttributeComp = OtherActor->FindComponentByClass<USAttributeComponent>();
		//USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		
		if (AttributeComp) 
		{
			FString CompName = AttributeComp->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Component Name: %s"), *CompName));
			
			AttributeComp->ApplyHealthChange(OtherActor, -50.0f); 
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Attribute Component not found!"));
		}
	}
	else
	{
		// Log a warning if OtherActor is invalid or self-reference
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Invalid OtherActor or self-reference."));
	}
}


