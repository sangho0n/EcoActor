// Fill out your copyright notice in the Description page of Project Settings.


#include "CrocoAnimInstance.h"

UCrocoAnimInstance::UCrocoAnimInstance()
{
	currSpeed = 0.0f;
	nextIdle = true;
	randomIdleIndx = 0;
}

void UCrocoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (nextIdle)
	{
		randomIdleIndx = FMath::FloorToInt(FMath::RandRange(0.0f, 3.99f));
		nextIdle = false;
	}

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		currSpeed = Pawn->GetVelocity().Size();
		if (!nextIdle && currSpeed <= 0.01f) 
		{ 
			nextIdle = true; 
		}
	}
}