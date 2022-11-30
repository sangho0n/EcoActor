// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomWaitTime.h"
#include "AnimalAIController.h"
#include "HunterAiController.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTask_GetRandomWaitTime::UBTTask_GetRandomWaitTime()
{
	NodeName = TEXT("GetRandomWaitTime");
}


EBTNodeResult::Type UBTTask_GetRandomWaitTime::ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory)
{

	EBTNodeResult::Type result = Super::ExecuteTask(OwnerCompoent, NodeMemory);

	auto ControllingPawn = OwnerCompoent.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;
	

	if(ControllingPawn->GetName().Contains(TEXT("Hunter")))
		OwnerCompoent.GetBlackboardComponent()->SetValueAsFloat(AHunterAiController::RandomWaitTimeKey, FMath::RandRange(0.0f, 5.0f));
	else
		OwnerCompoent.GetBlackboardComponent()->SetValueAsFloat(AAnimalAIController::RandomWaitTimeKey, FMath::RandRange(0.0f, 5.0f));

	return result;
}