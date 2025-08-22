// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTT_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


USBTT_RangedAttack::USBTT_RangedAttack()
{
	MaxBulletSpread = 2.0f;
}


EBTNodeResult::Type USBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (ensure(MyPawn==nullptr))
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (ensure(TargetActor==nullptr) || !USAttributeComponent::IsActorAlive(MyPawn))
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation()-MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Spawn Class : %s"), *TargetActor->GetName()));
		//DrawDebugSphere(GetWorld(), MuzzleLocation, 10.0f, 32, FColor::Red, false, 2.0f);
		
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParam.Instigator = MyPawn;

		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParam);
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		
	}

	return EBTNodeResult::Failed;
	
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}

