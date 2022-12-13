// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EcoActor.h"
#include "Level3CommonUI.h"
#include "CharacterStat.h"
#include "GameFramework/Character.h"
#include "EcoActorCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterDoValidAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateChanged, ECharacterState, NewState);
DECLARE_DELEGATE(FOnGameFailedDelegate);

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

	class UCharacterStat* CharacterStat;

	UFUNCTION(BlueprintCallable, Category = State)
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

	FOnCharacterStateChanged OnCharacterStateChanged;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta=(AllowPrivateAccess = true))
	ECharacterState CurrentState;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, Category=Camera)
	float BaseLookUpRate;

	float CharacterMaxSpeed;

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

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

	TSubclassOf<UUserWidget> BackgroundStoryWidgetClass;
	UUserWidget* BackgroundStoryWidget;

	TSubclassOf<UUserWidget> CommonUIClass;
	ULevel3CommonUI* CommonUI;

	TSubclassOf<UUserWidget> MenuWidgetClass;
	UUserWidget* MenuWidget;

	TSubclassOf<UUserWidget> SuccessWidgetClass;
	UUserWidget* SuccessWidget;

public:
	void SetCameraMode(EGameMode gameMode);

	bool isAttacking();

	bool isEquipped();

	void SetIsEquipping(bool newvalue);

	FOnCharacterDoValidAttack OnValidAttack;

	UCameraComponent* GetCameraComponent();

private:
	// combo ฐüทร
	int32 currentCombo;
	bool bCanNextCombo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsAttacking;
	bool bIsComboInputOn;
	bool bIsEquipped;
	bool bHoldKeyControl;
	FVector TargetPoint;
	float HitDamage = 5.0f;

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
	const int32 MaxBullets = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Gun, Meta=(AllowPrivateAccess=true))
	bool bIsEquipping;
	const float ShottableDistance = 3000.0f;
	float ShotDamage = 10.0f;

	UFUNCTION()
	void Shot();

	USkeletalMeshComponent* Gun;

public:
	void Equip();

private:
	bool bIsDead;
public:
	UFUNCTION()
	void SetDead();

	FOnGameFailedDelegate OnGameFailed;

	//buff
public:
	void ZebraBuff();
	void ElephantBuff();
	void CrocoBuff();
private:
	bool bOnZebraBuff;
	bool bOnCrocoBuff;

private:
	UFUNCTION()
	void Complete();
};

