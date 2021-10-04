// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShooterCharacter::Shoot() 
{
	AShooterCharacter* MainPlayer = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (MainPlayer && !MainPlayer->IsDead())
	{
		Gun->PullTrigger();
	}
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Gun = GetWorld()->SpawnActor<AGun>(GunType);
	GetMesh()->HideBoneByName(TEXT("weapon_r"),PBO_None);
	Gun->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageApplied= Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	Health= FMath::Clamp(Health - DamageApplied,0.f,MaxHealth);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		

	}
	return DamageApplied;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"),this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&AShooterCharacter::Shoot);
}

bool AShooterCharacter::IsDead() const
{
	return Health<=0.f;
}

float AShooterCharacter::GetHealthPercentage() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::MoveForward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector()* AxisValue);	
	
}

void AShooterCharacter::MoveRight(float AxisValue) 
{
	AddMovementInput(GetActorRightVector()* AxisValue);
}

/* void AShooterCharacter::LookUp(float AxisValue) 
{
	AddControllerPitchInput(AxisValue);
	UE_LOG(LogTemp,Warning,TEXT("Axis = %f"), AxisValue);
} */