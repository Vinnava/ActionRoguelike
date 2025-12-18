// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"

#include "SAttributeComponent.h"

//Sets default values
ASTargetDummy::ASTargetDummy()
{
 	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

}

void ASTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.RemoveDynamic(this, &ASTargetDummy::OnHealthChanged);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}


void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,float Delta)
{
	if (Delta < 0.0f)
	{
		StaticMesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
