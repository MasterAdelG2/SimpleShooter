// Fill out your copyright notice in the Description page of Project Settings.
#include "Gun.h"
#include "Components/StaticMeshComponent.h"
#include "particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AGun::PullTrigger() 
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh,TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound,Mesh,TEXT("MuzzleFlashSocket"));
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit,ShotDirection);
	if (bSuccess)
	{
		
		//DrawDebugPoint(GetWorld(),Hit.Location, 20.f , FColor::Red, true,0.1f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect,Hit.Location,ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),ImpactSound,Hit.Location);
		AActor* HitActor= Hit.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage,Hit,ShotDirection,nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage,DamageEvent,OwnerController,this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult & Hit,FVector& ShotDirection) 
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;
	FVector PVPLocation;
	FRotator PVPRotation;
	OwnerController->GetPlayerViewPoint(OUT PVPLocation,OUT PVPRotation);
	ShotDirection = -PVPRotation.Vector();
	FVector End = PVPLocation + PVPRotation.Vector() * MaxRange;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit,PVPLocation,End, ECollisionChannel::ECC_GameTraceChannel1,TraceParams);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn= Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

