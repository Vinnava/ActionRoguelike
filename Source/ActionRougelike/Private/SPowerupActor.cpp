// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "SCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComp);
	
	RespawnTime = 10.0f;

}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerupActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASPowerupActor::OnComponentBeginOverlap);
	
}


// Called every frame
void ASPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Logic in derived class
	
}


void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
	
}


void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
	
}


void ASPowerupActor::SetPowerupState(bool bIsNewActive)
{
	SetActorEnableCollision(bIsNewActive);

	RootComponent->SetVisibility(bIsNewActive, true);
	
}

void ASPowerupActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//APawn* InstigatorPawn = Cast<APawn>(OtherActor);
	ASCharacter* player = Cast<ASCharacter>(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("1.PowerUpOtherActor : %s"), *OtherActor->GetName()));
	if (!player) return;
	Execute_Interact(this, CastChecked<APawn>(OtherActor));
}
