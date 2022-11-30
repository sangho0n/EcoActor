// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRunAwayPos.h"
#include "AnimalAIController.h"
#include "Hunter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRunAwayPos::UBTTask_FindRunAwayPos()
{
	NodeName = TEXT("FindRunAwayPos");
}

EBTNodeResult::Type UBTTask_FindRunAwayPos::ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerCompoent, NodeMemory);

	auto Hunter = Cast<AHunter>(OwnerCompoent.GetBlackboardComponent()->GetValueAsObject(AAnimalAIController::RunAwayFromKey));
	auto ControllingPawn = OwnerCompoent.GetAIOwner()->GetPawn();
	FVector CurrPos = ControllingPawn->GetActorLocation();

	FVector OpositeVector = CurrPos - Hunter->GetActorLocation();
	OpositeVector.Z = 0.0f;
	OpositeVector.Normalize();

	OwnerCompoent.GetBlackboardComponent()->SetValueAsVector(AAnimalAIController::RunAwayPosKey, CurrPos+OpositeVector*500.0f);

	return EBTNodeResult::Succeeded;
}