// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AAnimalAIController::HomePosKey(TEXT("HomePos"));
const FName AAnimalAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AAnimalAIController::RandomWaitTimeKey(TEXT("RandomWaitTime"));
const FName AAnimalAIController::RunAwayFromKey(TEXT("RunAwayFrom"));
const FName AAnimalAIController::RunAwayPosKey(TEXT("RunAwayPos"));

AAnimalAIController::AAnimalAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBobj(TEXT("/Game/Main/AI/BB_Animal.BB_Animal"));
	if (BBobj.Succeeded())
	{
		BBAsset = BBobj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTobj(TEXT("/Game/Main/AI/BT_Animal.BT_Animal"));
	if (BTobj.Succeeded())
	{
		BTAsset = BTobj.Object;
	}
}

void AAnimalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			LOG(Warning, TEXT("Cannot run bb and bt"));
		}
	}
}