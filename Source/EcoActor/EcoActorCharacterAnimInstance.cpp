// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorCharacterAnimInstance.h"

UEcoActorCharacterAnimInstance::UEcoActorCharacterAnimInstance()
{
	CurrSpeed = 0.0f;
	bInAir = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>Combo(TEXT("/Game/Main/Anim/Fight/AM_FullCombo.AM_FullCombo"));
	if (Combo.Succeeded())
	{
		FullCombo = Combo.Object;
	}
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

void UEcoActorCharacterAnimInstance::AnimNotify_StartCombo()
{
	OnStartCombo.Broadcast();
}

void UEcoActorCharacterAnimInstance::AnimNotify_ComboHitCheck()
{
	OnComboHitCheck.Broadcast();
}

void UEcoActorCharacterAnimInstance::AnimNotify_NextComboCheck()
{
	OnNextComboCheck.Broadcast();
}

void UEcoActorCharacterAnimInstance::PlayComboMontage()
{
	if (!Montage_IsPlaying(FullCombo))
	{
		Montage_Play(FullCombo);
	}
}

void UEcoActorCharacterAnimInstance::JumpToComboSection(int32 currCombo)
{
	if (currCombo == 1)Montage_JumpToSection(getComboNontageSectionName(currCombo), FullCombo);
	else Montage_SetNextSection(getComboNontageSectionName(currCombo-1), getComboNontageSectionName(currCombo), FullCombo);
	LOG(Warning, TEXT("%d"), currCombo);
}

FName UEcoActorCharacterAnimInstance::getComboNontageSectionName(int32 currCombo)
{
	return FName(*FString::Printf(TEXT("Combo%d"), currCombo));
}