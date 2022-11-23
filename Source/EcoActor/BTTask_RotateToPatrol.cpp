// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToPatrol.h"
#include "AnimalAIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_RotateToPatrol::UBTTask_RotateToPatrol()
{
	NodeName = TEXT("RotateToPatrol");
}

EBTNodeResult::Type UBTTask_RotateToPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerCompoent, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerCompoent, NodeMemory);

	auto CurrentPawn = OwnerCompoent.GetAIOwner()->GetPawn();

	static FVector ToPatrolPosVec = OwnerCompoent.GetBlackboardComponent()->GetValueAsVector(AAnimalAIController::PatrolPosKey) - CurrentPawn->GetActorLocation();
	FVector LookPos = CurrentPawn->GetActorForwardVector();

	FVector Gap = ToPatrolPosVec - LookPos;

	while (Gap.Size() < 0.01f)
	{

		CurrentPawn->AddControllerYawInput(Gap.Size() / Gap.Size() * 0.0001f);
		LookPos = CurrentPawn->GetActorForwardVector();
		Gap = ToPatrolPosVec - LookPos;
	}

	return result;
}
