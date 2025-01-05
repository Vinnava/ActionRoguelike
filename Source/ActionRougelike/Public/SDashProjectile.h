// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Teleport")
	float TeleportDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Teleport")
	float DetonateDelay;
	
	FTimerHandle TimeHandler_DetonateDelay;
	
	FTimerHandle TimerHandler_TeleportDelay;
	

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void Explode_Implementation() override;

	UFUNCTION()
	void Teleport();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
