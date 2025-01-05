// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASDashProjectile::ASDashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASDashProjectile::OnActorOverlap);

	MovementComp->InitialSpeed=2000.0f;
	
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimeHandler_DetonateDelay, this, &ASDashProjectile::Explode_Implementation, DetonateDelay, false);
	
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASDashProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}


void ASDashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimeHandler_DetonateDelay);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Silver, FString::Printf(TEXT("Explode")));
	
	GetWorldTimerManager().SetTimer(TimerHandler_TeleportDelay, this, &ASDashProjectile::Teleport, TeleportDelay, false);
	
}


void ASDashProjectile::Teleport()
{
	GetWorldTimerManager().ClearTimer(TimerHandler_TeleportDelay);

	TObjectPtr<AActor> ActorToTeleport = GetInstigator();

	if (ensure(ActorToTeleport))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Teleport")));
		
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false,false);
	}
	
}
