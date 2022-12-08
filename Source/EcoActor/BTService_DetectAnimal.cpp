// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectAnimal.h"
#include "HunterAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"

UBTService_DetectAnimal::UBTService_DetectAnimal()
{
	NodeName = TEXT("DetectAnimal");
}

void UBTService_DetectAnimal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector CurrPos = ControllingPawn->GetActorLocation();
	float DetectRadius = 1300.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OveralResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OveralResults,
		CurrPos,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OveralResults)
		{
			auto DetectedPawn = OverlapResult.GetActor();
			if (nullptr != DetectedPawn && DetectedPawn->GetName().Contains("EcoActor") && !DetectedPawn->GetName().Contains("Character"))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AHunterAiController::DetectedAnimalKey, DetectedPawn);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(AHunterAiController::DetectedAnimalPosKey, DetectedPawn->GetActorLocation());
//#if ENABLE_DRAW_DEBUG
//				DrawDebugSphere(World, CurrPos, DetectRadius, 16, FColor::Red, false, 0.2f);
//				DrawDebugPoint(World, DetectedPawn->GetActorLocation(), 10.0f, FColor::Red, false, 0.2f);
//				DrawDebugLine(World, CurrPos, DetectedPawn->GetActorLocation(), FColor::Red, false, 0.2f);
//#endif
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AHunterAiController::DetectedAnimalKey, nullptr);
//#if ENABLE_DRAW_DEBUG
//	DrawDebugSphere(World, CurrPos, DetectRadius, 16, FColor::Blue, false, 0.2f);
//#endif
}
