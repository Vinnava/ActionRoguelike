// Fill out your copyright notice in the Description page of Project Settings.


//#include "SCharacter.h"
#include "ActionRougelike/Public/SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SInteractionComponet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponet>("InteractionComponent");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TimeToHitParamName = "TimeToHit";
	
	HandSocketName = "Muzzle_01";

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;
	
	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	*/
	
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::StopSprint);

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
	
}


void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
	
}


void ASCharacter::Jump()
{
	Super::Jump();
	
}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}


void ASCharacter::PrimaryAttack()
{
	StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	
}


void ASCharacter::BlackHoleAttack()
{
	StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
	
}


void ASCharacter::Dash()
{
	StartAttackEffects();

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.2f);
}


void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensure(ProjectileClass))
		SpawnProjectile(ProjectileClass);
}

void ASCharacter::StartSprint()
{
	ActionComp->StartActionByName(this, "Sprint");
}


void ASCharacter::Dash_TimeElapsed()
{
	if (ensure(DashProjectileClass))
		SpawnProjectile(DashProjectileClass);
}

void ASCharacter::StartAttackEffects()
{
	PlayAnimMontage(AttackAnim);
	
	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

}



void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

		/*
		//(Previously used Start&End)Not using this because the start location is the exact camera location. So, it's colliding with wall
		
		FVector TraceStart = CameraComp->GetComponentLocation();
		FVector TraceEnd = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000);
		*/

		FVector TraceStart = CameraComp->GetComponentLocation() + (CameraComp->GetForwardVector()*SpringArmComp->TargetArmLength);
		FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		
		FHitResult Hit;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		
		
		/*
		//Debug Line
		bool LineHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectQueryParams);
		FColor LineColor = LineHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 5.0f, 0.0f, 1.0f);*/
		

		
		FTransform SpawnTM;
		FRotator SpawnRot;
		
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, Params))
		{
			DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 32, FColor::Red, false, 2.0f);
			SpawnRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, Hit.Location);
		}
		else
		{
			SpawnRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, Hit.TraceEnd);
		}

		SpawnTM = FTransform(SpawnRot, HandLocation);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
                
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Spawn Class : %s"), *ClassToSpawn->GetName()));
	}
	
}


void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
	
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}

	if (Delta < 0.0f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Delta : %f"), Delta));
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}
