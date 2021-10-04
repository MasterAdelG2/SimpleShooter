// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

AShooterAIController::AShooterAIController() 
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShooterAIController::BeginPlay() 
{
    Super::BeginPlay();
    if (AIBehavior)
    {
        RunBehaviorTree(AIBehavior);
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime) 
{
     Super::Tick(DeltaTime);
    
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        return ControlledCharacter->IsDead();
    }
    return true;
}
