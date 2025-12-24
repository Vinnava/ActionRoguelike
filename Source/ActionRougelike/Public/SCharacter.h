// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponet;
class UAnimMontage;
class USAttributeComponent;
class UParticleSystem;

UCLASS()
class ACTIONROUGELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USInteractionComponet> InteractionComp;

	UPROPERTY(EditAnywhere, Category = Attack)
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	TObjectPtr<USAttributeComponent> AttributeComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	TObjectPtr<class USActionComponent> ActionComp;

	void PrimaryInteract();

	void PrimaryAttack();

	void BlackHoleAttack();

	void Dash();
	
	void StartSprint();
	
	void StopSprint();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual FVector GetPawnViewLocation() const override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void Jump();

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
	
	UCameraComponent* GetCameraComponent() const { return CameraComp; }
	
	USpringArmComponent* GetSpringArmComponent() const { return SpringArmComp; }
	
};



