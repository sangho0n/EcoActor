// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPosAnimal.h"
#include "AnimalAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPosAnimal::UBTTask_FindPatrolPosAnimal()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPosAnimal::ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerCompoent, NodeMemory);

	auto ControllingPawn = OwnerCompoent.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSys) return EBTNodeResult::Failed;

	FVector origin = OwnerCompoent.GetBlackboardComponent()->GetValueAsVector(AAnimalAIController::HomePosKey);
	FNavLocation nextPatrol;

	if (NavSys->GetRandomPointInNavigableRadius(origin, 2000.0f, nextPatrol))
	{
		OwnerCompoent.GetBlackboardComponent()->SetValueAsVector(AAnimalAIController::PatrolPosKey, nextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return result;
}
