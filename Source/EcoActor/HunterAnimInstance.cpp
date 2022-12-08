// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterAnimInstance.h"
#include "EcoActorCharacter.h"
#include "Kismet/GameplayStatics.h"

UHunterAnimInstance::UHunterAnimInstance()
{
	CurrSpeed = 0.0f;
	bIsHyperMode = false;
	bIsAttacking = false;
}

void UHunterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(Player))
	{
		Player->OnValidAttack.AddLambda([this]()->void {
			bIsAttacking = true;
			});
	}
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

void UHunterAnimInstance::AnimNotify_HunterAttack()
{
	OnHunterAttack.Broadcast();
}