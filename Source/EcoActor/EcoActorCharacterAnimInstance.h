// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Animation/AnimInstance.h"
#include "EcoActorCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartComboDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextComboCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboHitCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShotTriggeredDelegate);

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
	FVector CurrSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bInAir;

	UFUNCTION()
	void AnimNotify_StartCombo();

	UFUNCTION()
	void AnimNotify_NextComboCheck();

	UFUNCTION()
	void AnimNotify_ComboHitCheck();

	UFUNCTION()
	void AnimNotify_ShotTriggered();

	UFUNCTION()
	void AnimNotify_EquipStart();

	UFUNCTION()
	void AnimNotify_EquipEnd();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	UAnimMontage* FullCombo;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimationAsset* ShotAnim;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gun, Meta = (AllowPrivateAccess = true))
	bool bIsEquipped;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Gun, Meta = (AllowPrivateAccess = true))
	bool bIsEquipping;

public:
	void PlayComboMontage();

	void JumpToComboSection(int32 currCombo);

	FOnStartComboDelegate OnStartCombo;
	FOnNextComboCheckDelegate OnNextComboCheck;
	FOnShotTriggeredDelegate OnShotTriggered;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Attack")
	FOnComboHitCheckDelegate OnComboHitCheck;

private:
	FName getComboNontageSectionName(int32 currCombo);

	UAnimationAsset* DeadAnim;

public:
	void PlayDeadAnim();
};
