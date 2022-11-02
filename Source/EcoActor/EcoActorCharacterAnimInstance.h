// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Animation/AnimInstance.h"
#include "EcoActorCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartComboDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextComboCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ECOACTOR_API UEcoActorCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEcoActorCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float deltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bInAir;

	UFUNCTION()
	void AnimNotify_StartCombo();

	UFUNCTION()
	void AnimNotify_NextComboCheck();

	UFUNCTION()
	void AnimNotify_ComboHitCheck();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	UAnimMontage* FullCombo;

public:
	void PlayComboMontage();

	void JumpToComboSection(int32 currCombo);

	FOnStartComboDelegate OnStartCombo;
	FOnNextComboCheckDelegate OnNextComboCheck;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Attack")
	FOnComboHitCheckDelegate OnComboHitCheck;

private:
	FName getComboNontageSectionName(int32 currCombo);
};
