// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickUp/WarriorPckUpBase.h"
#include "Components/SphereComponent.h"

// Sets default values
AWarriorPckUpBase::AWarriorPckUpBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpCollisionSphere"));
	SetRootComponent(PickUpCollisionSphere);
	PickUpCollisionSphere->InitSphereRadius(50.0f);
	PickUpCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnPickUpCollisionSphereBeginOverlap);
}

void AWarriorPckUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

