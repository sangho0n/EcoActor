// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AHunterAiController::AHunterAiController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/Main/AI/BT_Hunter.BT_Hunter"));
	if (BT.Succeeded())
	{
		BTAsset = BT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("/Game/Main/AI/BB_Hunter.BB_Hunter"));
	if (BB.Succeeded())
	{
		BBAsset = BB.Object;
	}
}

const FName AHunterAiController::HomePosKey(TEXT("HomePos"));
const FName AHunterAiController::PatrolPosKey(TEXT("PatrolPos"));
const FName AHunterAiController::RandomWaitTimeKey(TEXT("RandomWaitTime"));
const FName AHunterAiController::DetectedAnimalKey(TEXT("DetectedAnimal"));
const FName AHunterAiController::DetectedAnimalPosKey(TEXT("DetectedAnimalPos"));

void AHunterAiController::OnPossess(APawn* InPawn)
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