// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen() 
{
    NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
        return;
    AShooterCharacter* PlayerPawn = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
    if (PlayerPawn == nullptr)
    {
        return ;
    }
    if (AIController->LineOfSightTo(PlayerPawn))
    {
        UE_LOG(LogTemp, Warning,TEXT("Seting Value"));
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(),PlayerPawn);
    }
    else
    {
        UE_LOG(LogTemp, Error,TEXT("Clearing Value"));
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); 
    } 
}