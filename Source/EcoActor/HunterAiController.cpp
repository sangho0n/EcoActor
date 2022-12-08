// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Hunter.h"
#include "Kismet/GameplayStatics.h"

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
const FName AHunterAiController::WasAttackedKey(TEXT("WasAttacked")); 
const FName AHunterAiController::PlayerPosKey(TEXT("PlayerPos"));

void AHunterAiController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AHunterAiController::BeginPlay()
{
	Super::BeginPlay();

	ControlledPawn = Cast<AHunter>(this->GetPawn());
	Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Player))
	{
		Player->OnValidAttack.AddLambda([this]() -> void {
			LOG(Warning, TEXT("hunter ai controller begin play add lambda"));
			ControlledPawn->SetAttacked();
			this->GetBlackboardComponent()->SetValueAsBool(AHunterAiController::WasAttackedKey, ControlledPawn->GetAttacked());
			this->GetBlackboardComponent()->SetValueAsVector(AHunterAiController::PlayerPosKey, Player->GetActorLocation());
			});
	}

	auto ControlledPawn = Cast<AHunter>(this->GetPawn());
	ControlledPawn->SetPlayer();

}

void AHunterAiController::Tick(float DeltaSeconds)
{
	if (ControlledPawn->GetAttacked())
	{
		this->GetBlackboardComponent()->SetValueAsVector(AHunterAiController::PlayerPosKey, Player->GetActorLocation());
	}
}

void AHunterAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AHunterAiController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (RunBehaviorTree(BTAsset))
		{
			LOG(Warning, TEXT("can run bb and bt"));
		}
		else
		{
			LOG(Warning, TEXT("Cannot run bb and bt"));
		}
	}
}

void AHunterAiController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}