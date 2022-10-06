// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorCharacterAnimInstance.h"

UEcoActorCharacterAnimInstance::UEcoActorCharacterAnimInstance()
{
	CurrSpeed = 0.0f;
	bInAir = false;
}

void UEcoActorCharacterAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	Super::NativeUpdateAnimation(deltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			bInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}