// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EcoActor.h"
#include "Animation/AnimInstance.h"
#include "CrocoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECOACTOR_API UCrocoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCrocoAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float currSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool nextIdle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int32 randomIdleIndx;

};
