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

//////////////////////////////////////////////////////////////////////////
// AEcoActorCharacter


AEcoActorCharacter::AEcoActorCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();

	// skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NATHAN(TEXT("/Game/Scanned3DPeoplePack/RP_Character/rp_nathan_rigged_003_ue4/rp_nathan_rigged_003_ue4.rp_nathan_rigged_003_ue4"));
	if (SK_NATHAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_NATHAN.Object);
	}

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

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

	// Animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> JUMP_AND_RUN_ANIM(TEXT("/Game/Main/Anim/AnimBP_JumpRun.AnimBP_JumpRun_C"));
	if (JUMP_AND_RUN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(JUMP_AND_RUN_ANIM.Class);
		LOG(Warning, TEXT("set aninm"));
	}

	setPlayerMode(EGameMode::ThirdPerson);

	currentCombo = 0;
	bCanNextCombo = false;
	bIsComboInputOn = false;
	bIsEquipped = false;
	bIsAttacking = false;
	bHoldKeyControl = false;
	TargetPoint = FVector::ZeroVector;
}

void AEcoActorCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	AnimInstance->OnComboHitCheck.AddDynamic(this, &AEcoActorCharacter::ComboHit);
}

void AEcoActorCharacter::setPlayerMode(EGameMode newGameMode)
{
	switch (newGameMode)
	{
	case EGameMode::ThirdPerson:
		// Don't rotate when the controller rotates. Let that just affect the camera.
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;

		CameraBoom->bInheritPitch = true;
		CameraBoom->bInheritYaw = true;
		CameraBoom->bInheritRoll = true;
		CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
		CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
		FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		break;
	case EGameMode::FirstPerson:

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
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AEcoActorCharacter::Attack);
}

void AEcoActorCharacter::Jump()
{
	if (bHoldKeyControl) return;
	Super::Jump();
}

void AEcoActorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AEcoActorCharacter::MoveForward(float Value)
{
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
	if (!bIsEquipped)
	{
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
	else
	{

	}
}

void AEcoActorCharacter::Tick(float deltaSeconds)
{
	if (bHoldKeyControl)
	{
		// move that point
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetPoint, deltaSeconds, 3.0f), true);
	}
}

void AEcoActorCharacter::OnComboMontageEnded(UAnimMontage* Mont, bool bInterupped)
{
	bHoldKeyControl = false;
	bIsComboInputOn = false;
	bIsAttacking = false;
	bCanNextCombo = false;
	currentCombo = 0;
}

void AEcoActorCharacter::ComboHit()
{

}