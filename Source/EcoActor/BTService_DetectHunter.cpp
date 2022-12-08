#include "BTService_DetectHunter.h"
#include "AnimalAIController.h"
#include "Hunter.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"


UBTService_DetectHunter::UBTService_DetectHunter()
{
	NodeName = TEXT("DetectHunter");
	Interval = 1.0f;
}

void UBTService_DetectHunter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector CurrPos = ControllingPawn->GetActorLocation();
	float DetectRadius = 900.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OveralResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OveralResults,
		CurrPos,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OveralResults)
		{
			auto DetectedPawn = Cast<AHunter>(OverlapResult.GetActor());
			if (nullptr != DetectedPawn)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAnimalAIController::RunAwayFromKey, DetectedPawn);
//#if ENABLE_DRAW_DEBUG
//				DrawDebugSphere(World, CurrPos, DetectRadius, 16, FColor::Red, false, 0.2f);
//				DrawDebugPoint(World, DetectedPawn->GetActorLocation(), 10.0f, FColor::Red, false, 0.2f);
//				DrawDebugLine(World, CurrPos, DetectedPawn->GetActorLocation(), FColor::Red, false, 0.2f);
//#endif
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAnimalAIController::RunAwayFromKey, nullptr);
//#if ENABLE_DRAW_DEBUG
//	DrawDebugSphere(World, CurrPos, DetectRadius, 16, FColor::Blue, false, 0.2f);
//#endif
}