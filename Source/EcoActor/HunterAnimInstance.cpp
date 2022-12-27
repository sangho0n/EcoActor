// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterAnimInstance.h"
#include "EcoActorCharacter.h"
#include "Kismet/GameplayStatics.h"

UHunterAnimInstance::UHunterAnimInstance()
{
	CurrSpeed = 0.0f;
	bIsHyperMode = false;
	bIsAttacking = false;

	ConstructorHelpers::FObjectFinder<UAnimationAsset> DEADANIM(TEXT("/Game/Main/Anim/Hunter/Hunter_Death_3.Hunter_Death_3"));
	if (DEADANIM.Succeeded())
	{
		DeadAnim = DEADANIM.Object;
	}

	bHit = false;
}

void UHunterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UHunterAnimInstance::NativeUpdateAnimation(float deltaSeconds)
{
	Super::NativeUpdateAnimation(deltaSeconds);

	auto pawn =TryGetPawnOwner();
	if (IsValid(pawn))
	{
		CurrSpeed = pawn->GetVelocity().Size();
	}
}

void UHunterAnimInstance::AnimNotify_HunterAttack()
{
	OnHunterAttack.Broadcast();
}

void UHunterAnimInstance::PlayDeadAnim()
{
	auto pawn = Cast<ACharacter>(TryGetPawnOwner());
	pawn->GetMesh()->PlayAnimation(DeadAnim, false);
}


void UHunterAnimInstance::SetHunterToAttack()
{
	bIsAttacking = true;
}

void UHunterAnimInstance::SetHunterToHyperMode()
{
	bIsHyperMode = true;
}

void UHunterAnimInstance::SetHit(bool isHit)
{
	bHit = isHit;
	if (bHit)
	{
		LOG(Warning, TEXT("True hit"));
	}
	else
		LOG(Warning, TEXT("Not hit"));
}