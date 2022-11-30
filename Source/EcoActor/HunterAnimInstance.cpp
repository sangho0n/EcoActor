// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterAnimInstance.h"

UHunterAnimInstance::UHunterAnimInstance()
{
	CurrSpeed = 0.0f;
	bIsHyperMode = false;
	bIsAttacking = false;
}

void UHunterAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	Super::NativeUpdateAnimation(deltaSeconds);

	auto pawn = TryGetPawnOwner();
	if (IsValid(pawn))
	{
		CurrSpeed = pawn->GetVelocity().Size();
	}
}