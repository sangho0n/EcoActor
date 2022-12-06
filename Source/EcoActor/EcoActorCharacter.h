// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EcoActor.h"
#include "GameFramework/Character.h"
#include "EcoActorCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterDoValidAttack);

UCLASS(config=Game)
class AEcoActorCharacter : public ACharacter
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AEcoActorCharacter();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float deltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void Jump() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Attack();

	void AttackWithGunStart();

	void AttackWithGunStop();

	void QPressed();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	enum EGameMode
	{
		ThirdPerson,
		ShotMode
	};
	bool hasInitialized = false;
	TSubclassOf<UUserWidget> MenuWidgetClass;
	UUserWidget* MenuWidget;

public:
	void setPlayerMode(EGameMode gameMode);

	bool isAttacking();

	bool isEquipped();

	void SetIsEquipping(bool newvalue);

	FOnCharacterDoValidAttack OnValidAttack;

private:
	// combo ����
	int32 currentCombo;
	bool bCanNextCombo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsAttacking;
	bool bIsComboInputOn;
	bool bIsEquipped;
	bool bHoldKeyControl;
	FVector TargetPoint;
	const float HitDamage = 5.0f;

	class UEcoActorCharacterAnimInstance* AnimInstance;
protected:
	UFUNCTION()
	void Hit();
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	float HitRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float HitRadius;

private:
	UFUNCTION()
	void OnComboMontageEnded(UAnimMontage* Montage, bool bInterupted);

private:
	// gun
	int32 LeftBullets;
	const int32 MaxBullets = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Gun, Meta=(AllowPrivateAccess=true))
	bool bIsEquipping;
	const float ShottableDistance = 3000.0f;
	const float ShotDamage = 10.0f;

	UFUNCTION()
	void Shot();

	USkeletalMeshComponent* Gun;

public:
	void Equip();
};

