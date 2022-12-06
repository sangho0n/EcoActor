// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "AnimalAIController.h"
#include "HunterAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerCompoent, NodeMemory);

	auto ControllingPawn = OwnerCompoent.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSys) return EBTNodeResult::Failed;

	FVector origin;
	if (ControllingPawn->GetName().Contains(TEXT("Hunter")))
		origin = OwnerCompoent.GetBlackboardComponent()->GetValueAsVector(AHunterAiController::HomePosKey);
	else
		origin = OwnerCompoent.GetBlackboardComponent()->GetValueAsVector(AAnimalAIController::HomePosKey);

	FNavLocation nextPatrol;

	if (NavSys->GetRandomPointInNavigableRadius(origin, 2000.0f, nextPatrol))
	{
		if (ControllingPawn->GetName().Contains(TEXT("Hunter")))
			OwnerCompoent.GetBlackboardComponent()->SetValueAsVector(AHunterAiController::PatrolPosKey, nextPatrol.Location);
		else
			OwnerCompoent.GetBlackboardComponent()->SetValueAsVector(AAnimalAIController::PatrolPosKey, nextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return result;
}
