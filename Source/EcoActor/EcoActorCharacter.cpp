// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "EcoActorCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EcoActorCharacterAnimInstance.h"
#include "EcoActorCharacterState.h"
#include "UserWidget.h"
//#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AEcoActorCharacter


AEcoActorCharacter::AEcoActorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	// skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NATHAN(TEXT("/Game/Scanned3DPeoplePack/RP_Character/rp_nathan_rigged_003_ue4/rp_nathan_rigged_003_ue4.rp_nathan_rigged_003_ue4"));
	if (SK_NATHAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_NATHAN.Object);
	}

	// set our turn rates for input
	BaseTurnRate = 75.f;
	BaseLookUpRate = 75.f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.9f;
	GetCharacterMovement()->StandingDownwardForceScale = 3.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// character stat component
	CharacterStat = CreateDefaultSubobject<UCharacterStat>(TEXT("CharacterState"));
	CharacterStat->SetMaxHP(200.0f);

	// Animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> JUMP_AND_RUN_ANIM(TEXT("/Game/Main/Anim/Character/AnimBP_JumpRun.AnimBP_JumpRun_C"));
	if (JUMP_AND_RUN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(JUMP_AND_RUN_ANIM.Class);
		LOG(Warning, TEXT("set aninm"));
	}

	SetCameraMode(EGameMode::ThirdPerson);

	currentCombo = 0;
	bCanNextCombo = false;
	bIsComboInputOn = false;
	bIsEquipped = false;
	bIsAttacking = false;
	bHoldKeyControl = false;
	TargetPoint = FVector::ZeroVector;
	HitRange = 200.0f;
	HitRadius = 50.0f;

	LeftBullets = MaxBullets; 
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun")); 
	bIsEquipping = false;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RIFLE(TEXT("/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B.Assault_Rifle_B"));
	if (RIFLE.Succeeded())
	{
		Gun->SetSkeletalMesh(RIFLE.Object);
	}

	Gun->SetVisibility(false);

	hasInitialized = true;


	ConstructorHelpers::FClassFinder<UUserWidget> COMMONUI(TEXT("/Game/Main/UI/BP_Level3CommonUI.BP_Level3CommonUI_C"));
	if (COMMONUI.Succeeded())
	{
		CommonUIClass = COMMONUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MENU_UI(TEXT("/Game/Level2/OceanCleaning/BP_Menu.BP_Menu_C"));
	if (MENU_UI.Succeeded())
	{
		MenuWidgetClass = MENU_UI.Class;
	}
	
	//SetActorHiddenInGame(true);
	SetActorHiddenInGame(false);
	//CommonUI->SetVisibility(ESlateVisibility::Hidden);
	bCanBeDamaged = false;
}

void AEcoActorCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(CommonUIClass))
	{
		// À§Á¬À» »ý¼ºÇÑ´Ù
		CommonUI = Cast<ULevel3CommonUI>(CreateWidget(GetWorld(), CommonUIClass));

		if (IsValid(CommonUI))
		{
			// À§Á¬À» ºäÆ÷Æ®¿¡ Ãß°¡ÇÑ´Ù
			CommonUI->AddToViewport();

			auto palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			palyerController->SetInputMode(FInputModeGameOnly());
			palyerController->bShowMouseCursor = false;
			palyerController->bEnableClickEvents = false;
			palyerController->bEnableMouseOverEvents = false;
		}
	}

	CommonUI->BindCharacterStat(CharacterStat);
	CommonUI->SetFistMode();
	CharacterStat->SetMaxHP(200.0f);

	// Æ©Åä¸®¾ó À§Á¬ ºÒ·¯¿À±â

	//SetCharacterState(ECharacterState::LOADING);
}

void AEcoActorCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		SetActorHiddenInGame(true);
		//CommonUI->SetVisibility(ESlateVisibility::Hidden);
		bCanBeDamaged = false;
		// todo : Æ©Åä¸®¾ó À§Á¬ ¶ç¿ì±â
		break;
	}
	case ECharacterState::READY:
	{
		// todo : Æ©Åä¸®¾ó À§Á¬ Á¦°Å
		SetActorHiddenInGame(false);
		//CommonUI->SetVisibility(ESlateVisibility::Visible);
		bCanBeDamaged = true;

		//AEcoActorCharacterState->OnHPIsZero
	}
	}
}

void AEcoActorCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UEcoActorCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnNextComboCheck.AddLambda([this]()->void
	{
			bHoldKeyControl = false;
			bCanNextCombo = false;
			if (bIsComboInputOn)
			{
				bCanNextCombo = true;
				bIsComboInputOn = false;
				currentCombo = FMath::Clamp<int32>(currentCombo + 1, 1, 4);
				AnimInstance->JumpToComboSection(currentCombo);
			}
		});

	AnimInstance->OnMontageEnded.AddDynamic(this, &AEcoActorCharacter::OnComboMontageEnded);

	AnimInstance->OnStartCombo.AddLambda([this]()->void {
		bHoldKeyControl = true;
		const FVector CurrPoint = GetActorLocation();
		const FRotator Rotation = GetActorRotation();
		FRotator NormedRot = Rotation.GetNormalized();

		// get direction vector
		const FVector Direction = NormedRot.Vector();
		TargetPoint = CurrPoint + Direction * 300.0f;
		});

	AnimInstance->OnComboHitCheck.AddDynamic(this, &AEcoActorCharacter::Hit);

	AnimInstance->OnShotTriggered.AddDynamic(this, &AEcoActorCharacter::Shot);

	CharacterStat->OnHPChanged.AddLambda([this]() -> void{
		// hp º¯ÇÏµµ·Ï
		});
}

void AEcoActorCharacter::SetCameraMode(EGameMode newGameMode)
{
	switch (newGameMode)
	{
	case EGameMode::ThirdPerson:
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		// Don't rotate when the controller rotates. Let that just affect the camera.
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;

		CameraBoom->bInheritPitch = true;
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritRoll = true;
		CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		if(!hasInitialized)
			FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		else
			FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		break;
	case EGameMode::ShotMode:
		GetCharacterMovement()->bOrientRotationToMovement = false; 		
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = true;

		CameraBoom->SetRelativeLocation(FVector(-50.0f, 40.0f, 60.0f));
		CameraBoom->bInheritPitch = true;
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritRoll = true;
		CameraBoom->TargetArmLength = 30.0f;
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		if (!hasInitialized)
			FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		else
			FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		break;
	}
}

void AEcoActorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AEcoActorCharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEcoActorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEcoActorCharacter::MoveRight);

	// "turn" handles devices that provide an absolute delta, such as a mouse.
	PlayerInputComponent->BindAxis("Turn", this, &AEcoActorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AEcoActorCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AEcoActorCharacter::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AEcoActorCharacter::AttackWithGunStart);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AEcoActorCharacter::AttackWithGunStop);

	PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &AEcoActorCharacter::QPressed);
}

void AEcoActorCharacter::Jump()
{
	if (bHoldKeyControl || bIsEquipping) return;
	Super::Jump();
}

void AEcoActorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEcoActorCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEcoActorCharacter::MoveForward(float Value)
{
	if (bIsEquipping) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (bHoldKeyControl) return;
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEcoActorCharacter::MoveRight(float Value)
{
	if (bIsEquipping) return;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		if (bHoldKeyControl) return;
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AEcoActorCharacter::Attack()
{
	if (bIsEquipped || bIsEquipping) return;

	if (nullptr != AnimInstance)
	{
		if (bIsAttacking)
		{
			if(bCanNextCombo)
				bIsComboInputOn = true;
		}
		else
		{
			bIsComboInputOn = false;
			bCanNextCombo = true;
			currentCombo = FMath::Clamp<int32>(currentCombo + 1, 1, 4);
			AnimInstance->PlayComboMontage();
			AnimInstance->JumpToComboSection(currentCombo);

			bIsAttacking = true;
		}
	}
}

void AEcoActorCharacter::AttackWithGunStart()
{
	if (!bIsEquipped || bIsEquipping) return;

	bIsAttacking = true;
	GetCharacterMovement()->MaxWalkSpeed /= 2.0f;
	SetCameraMode(EGameMode::ShotMode);
}

void AEcoActorCharacter::AttackWithGunStop()
{
	if (!bIsEquipped || bIsEquipping) return;

	bIsAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
	SetCameraMode(EGameMode::ThirdPerson);
}

void AEcoActorCharacter::Tick(float deltaSeconds)
{
	if (bHoldKeyControl)
	{
		// move that point
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetPoint, deltaSeconds, 3.0f), true);
	}

	if (bIsEquipped && bIsAttacking && LeftBullets == 0)
	{
		AttackWithGunStop();
		bIsEquipped = false;
		Gun->SetVisibility(false);
		CommonUI->SetFistMode();
	}
}

bool AEcoActorCharacter::isAttacking()
{
	return bIsAttacking;
}

void AEcoActorCharacter::SetIsEquipping(bool newVal)
{
	bIsEquipping = newVal;
}

void AEcoActorCharacter::OnComboMontageEnded(UAnimMontage* Mont, bool bInterupped)
{
	bHoldKeyControl = false;
	bIsComboInputOn = false;
	bIsAttacking = false;
	bCanNextCombo = false;
	currentCombo = 0;
}

void AEcoActorCharacter::Hit()
{
	FVector ForwardVec = GetActorForwardVector();
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + ForwardVec * HitRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(HitRadius),
		Params
	);

	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(HitDamage, DamageEvent, GetController(), this);

		OnValidAttack.Broadcast();
	}

//#if ENABLE_DRAW_DEBUG
//	FVector TraceVec = ForwardVec * HitRange;
//
//	if (bResult)
//	{
//		DrawDebugCapsule(
//			GetWorld(),
//			GetActorLocation() + TraceVec * 0.5f,
//			TraceVec.Size()*0.5f,
//			HitRadius,
//			FRotationMatrix::MakeFromZ(TraceVec).ToQuat(),
//			FColor::Red,
//			false,
//			3.0f
//		);
//	}
//	else
//	{
//		DrawDebugCapsule(
//			GetWorld(),
//			GetActorLocation() + TraceVec * 0.5f,
//			TraceVec.Size() * 0.5f,
//			HitRadius,
//			FRotationMatrix::MakeFromZ(TraceVec).ToQuat(),
//			FColor::Green,
//			false,
//			3.0f
//		);
//
//	}
//#endif
}

bool AEcoActorCharacter::isEquipped()
{
	return bIsEquipped;
}

void AEcoActorCharacter::Shot()
{
	LeftBullets--;
	LOG(Warning, TEXT("Bullet left : %d"), LeftBullets);
	CommonUI->UpdateLeftBullet(LeftBullets, MaxBullets);

	FVector CameraVec = FollowCamera->GetComponentLocation(); // cast ray from camera
	FVector LookAtVec = GetControlRotation().Vector(); // to look at point(center of monitor)
	LookAtVec.Normalize();
	
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraVec,
		CameraVec + LookAtVec * ShottableDistance,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params
	);

	if (bResult)
	{
		// get target location
		FVector TargetLocation = HitResult.ImpactPoint;
		// and make vertical circle
		float DistanceToTarget = HitResult.Distance;
		float CircleRadius = DistanceToTarget * 0.03f;
		// get random point from radius and angle
		float RandomRadius = FMath::FRand() * CircleRadius;
		float RandomAngle = FMath::FRand() * 360.0f;
		FVector HittedActorUpVec = HitResult.GetActor()->GetActorUpVector();
		FVector RandomAngleVector = HittedActorUpVec.RotateAngleAxis(RandomAngle, LookAtVec);

		FVector RandomPointInWorld = CameraVec + LookAtVec * DistanceToTarget + RandomAngleVector * RandomRadius;
		// linetrace to that point
		bResult = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			CameraVec,
			RandomPointInWorld + LookAtVec*3.0f,
			ECollisionChannel::ECC_GameTraceChannel1,
			Params
		);
		if (bResult)
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(ShotDamage, DamageEvent, GetController(), this);

			OnValidAttack.Broadcast();
		}

//#if ENABLE_DRAW_DEBUG
//		DrawDebugCircle(
//			GetWorld(),
//			CameraVec + LookAtVec*DistanceToTarget,
//			CircleRadius,
//			16,
//			bResult ? FColor::Red : FColor::Green,
//			false,
//			0.7f,
//			0U,
//			0.0f,
//			GetActorRightVector(),
//			GetActorUpVector()
//		); 
//		DrawDebugLine(
//			GetWorld(),
//			CameraVec,
//			CameraVec + LookAtVec * ShottableDistance,
//			FColor::Blue,
//			false,
//			5.0f
//		);
//#endif
	}
}

float AEcoActorCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FinalDamage = DamageAmount;

	LOG(Warning, TEXT("player took dam : %f"), FinalDamage);
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void AEcoActorCharacter::Equip()
{
	if (bIsAttacking) return;

	FName WeaponSocket(TEXT("hand_rSocket"));

	Gun->SetVisibility(true);
	Gun->SetCollisionProfileName("NoCollision");
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	bIsEquipped = true;
	LeftBullets = MaxBullets;
	CommonUI->SetGunMode();
}

void AEcoActorCharacter::QPressed()
{
	MenuWidget = CreateWidget(GetWorld(), MenuWidgetClass);
	if (IsValid(MenuWidget))
	{
		auto palyerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		MenuWidget->AddToViewport();

		palyerController->SetInputMode(FInputModeUIOnly());
		palyerController->bShowMouseCursor = true;
		palyerController->bEnableClickEvents = true;
		palyerController->bEnableMouseOverEvents = true;
	}
}

