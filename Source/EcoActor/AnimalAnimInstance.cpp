// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalAnimInstance.h"

UAnimalAnimInstance::UAnimalAnimInstance()
{
	currSpeed = 0.0f;
}

void UAnimalAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		currSpeed = Pawn->GetVelocity().Size();
	}
}