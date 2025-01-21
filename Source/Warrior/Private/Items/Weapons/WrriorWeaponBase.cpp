// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WrriorWeaponBase.h"
#include "Components/BoxComponent.h"
#include "WrriorFunctionLibrary.h"

#include "WarriorDebugHelper.h"

AWrriorWeaponBase::AWrriorWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollisionBox->SetupAttachment(GetRootComponent());
	WeaponCollisionBox->SetBoxExtent(FVector(20.0f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBegineOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);
}


void AWrriorWeaponBase::OnCollisionBoxBegineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forget to assign an Instigator as the Owning Pawn for the Weapon : %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UWrriorFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}


void AWrriorWeaponBase::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("Forget to assign an Instigator as the Owning Pawn for the Weapon : %s"), *GetName());

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (UWrriorFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}