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
#include "Hunter.h"
#include "Spawner.h"
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

	//spawner = GetWorld()->SpawnActor<ASpawner>(GetActorLocation(), FRotator::ZeroRotator);
	//spawner->K2_AttachRootComponentToActor(this);

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


	static ConstructorHelpers::FClassFinder<UUserWidget> COMMONUI(TEXT("/Game/Main/UI/BP_Level3CommonUI.BP_Level3CommonUI_C"));
	if (COMMONUI.Succeeded())
	{
		CommonUIClass = COMMONUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MENU_UI(TEXT("/Game/Main/BP_Menu2.BP_Menu2_C"));
	if (MENU_UI.Succeeded())
	{
		MenuWidgetClass = MENU_UI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> BG_UI(TEXT("/Game/Tutorial/Level3_BS/level3_BackgroundStory.level3_BackgroundStory_C"));
	if (BG_UI.Succeeded())
	{
		BackgroundStoryWidgetClass = BG_UI.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SC_UI(TEXT("/Game/Main/UI/BP_MissionComplete_UI.BP_MissionComplete_UI_C"));
	if (SC_UI.Succeeded())
	{
		SuccessWidgetClass = SC_UI.Class;
	}
	
	//SetActorHiddenInGame(true);
	SetActorHiddenInGame(false);
	//CommonUI->SetVisibility(ESlateVisibility::Hidden);
	bCanBeDamaged = true;
	bIsDead = false;

	SetCharacterState(ECharacterState::PREINIT);
	bOnZebraBuff = false;
	bOnCrocoBuff = false;
	CharacterMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// sound
	static ConstructorHelpers::FObjectFinder<USoundBase> BGMSOUND(TEXT("/Game/StylizedForestPack/Sounds/Ambiances/Cues/Forest_Daytime/Forest_Daytime_1_Cue.Forest_Daytime_1_Cue"));
	if (BGMSOUND.Succeeded())
	{
		BGM = BGMSOUND.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> PICKUPSOUND(TEXT("/Game/MilitaryWeapDark/Sound/Rifle/Rifle_Raise_Cue.Rifle_Raise_Cue"));
	if (PICKUPSOUND.Succeeded())
	{
		PickupGunSound = PICKUPSOUND.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> SHOTSOUND(TEXT("/Game/MilitaryWeapDark/Sound/Rifle/RifleB_FireLoop_Cue.RifleB_FireLoop_Cue"));
	if (SHOTSOUND.Succeeded())
	{
		ShotGunSound = SHOTSOUND.Object;
	}
	
	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	PickupGunAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupGunAudioComponent"));
	ShotGunAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ShotGunAudioComponent"));
	BGMComponent->SetupAttachment(RootComponent);
	PickupGunAudioComponent->SetupAttachment(RootComponent);
	ShotGunAudioComponent->SetupAttachment(RootComponent);
}

void AEcoActorCharacter::BeginPlay()
{
	Super::BeginPlay();

	BGMComponent->SetSound(BGM);
	BGMComponent->Stop();
	PickupGunAudioComponent->SetSound(PickupGunSound);
	PickupGunAudioComponent->Stop();
	ShotGunAudioComponent->SetSound(ShotGunSound);
	ShotGunAudioComponent->Stop();

	spawner = GetWorld()->SpawnActor<ASpawner>(GetActorLocation(), FRotator::ZeroRotator);
	spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	if (IsValid(CommonUIClass))
	{
		// ������ �����Ѵ�
		CommonUI = Cast<ULevel3CommonUI>(CreateWidget(GetWorld(), CommonUIClass));

		if (IsValid(CommonUI))
		{
			// ������ ����Ʈ�� �߰��Ѵ�
			CommonUI->AddToViewport();

			auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
		}
	}

	CommonUI->BindCharacterStat(CharacterStat);
	CommonUI->SetFistMode();
	CharacterStat->SetMaxHP(200.0f);

	SetCharacterState(ECharacterState::LOADING);

	CharacterStat->OnScoreChanged.AddLambda([this]()->void {
		CommonUI->UpdateScore();
		});


	CommonUI->OnTimesUp.AddUFunction(this, FName("SetCharacterState"), ECharacterState::DEAD);
}

UCameraComponent* AEcoActorCharacter::GetCameraComponent()
{
	return FollowCamera;
}

ASpawner* AEcoActorCharacter::GetSpawner()
{
	return spawner;
}

void AEcoActorCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::PREINIT:
	{
		bIsDead = false;
		bCanBeDamaged = false;
		break;
	}
	case ECharacterState::LOADING:
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		SetActorHiddenInGame(true);
		CommonUI->SetVisibility(ESlateVisibility::Hidden);
		bCanBeDamaged = false;
		// todo : Ʃ�丮�� ���� ����
		BackgroundStoryWidget = CreateWidget(GetWorld(), BackgroundStoryWidgetClass);
		if (IsValid(BackgroundStoryWidget))
		{
			auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			BackgroundStoryWidget->AddToViewport();


			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
		}
		break;
	}
	case ECharacterState::READY:
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		SetActorHiddenInGame(false);
		CommonUI->SetVisibility(ESlateVisibility::Visible);

		PlayerController->SetPause(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		LOG(Warning, TEXT("ready"));
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		bCanBeDamaged = true;
		CommonUI->PlayTimer();
		BGMComponent->Play();

		break;
	}
	case ECharacterState::DEAD:
	{
		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		SetActorHiddenInGame(false);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		CommonUI->SetVisibility(ESlateVisibility::Hidden);
		bCanBeDamaged = false;
		bIsDead = true;
		AnimInstance->PlayDeadAnim();
		OnGameFailed.Execute();
		BGMComponent->Stop();

		break;
	}
	}

	OnCharacterStateChanged.Broadcast(CurrentState);
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


	CharacterStat->OnHPIsZero.AddDynamic(this, &AEcoActorCharacter::SetDead);

	CharacterStat->OnScoreReachTop.AddDynamic(this, &AEcoActorCharacter::Complete);
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
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxSpeed / 2;
	SetCameraMode(EGameMode::ShotMode);
	ShotGunAudioComponent->Play();
}

void AEcoActorCharacter::AttackWithGunStop()
{
	if (!bIsEquipped || bIsEquipping) return;

	bIsAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxSpeed;
	SetCameraMode(EGameMode::ThirdPerson);
	ShotGunAudioComponent->Stop();
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
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + ForwardVec * HitRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(HitRadius),
		Params
	);

	if (bResult)
	{
		for (auto HitResult : HitResults)
		{
			auto Hunter = Cast<AHunter>(HitResult.Actor);
			Hunter->OnValidAttack();
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(HitDamage, DamageEvent, GetController(), this);
		}

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
			auto Hunter = Cast<AHunter>(HitResult.Actor);
			Hunter->OnValidAttack();

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(ShotDamage, DamageEvent, GetController(), this);
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
	if (!bCanBeDamaged) return 0.0f;

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
	PickupGunAudioComponent->Play();
}

void AEcoActorCharacter::QPressed()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	MenuWidget = CreateWidget(GetWorld(), MenuWidgetClass);
	if (IsValid(MenuWidget))
	{
		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		MenuWidget->AddToViewport();

		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

void AEcoActorCharacter::SetDead()
{
	SetCharacterState(ECharacterState::DEAD);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AEcoActorCharacter::ZebraBuff()
{
	FTimerHandle TimerHandle;
	if (bOnZebraBuff)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		goto SetBuff;
	}

	bOnZebraBuff = true;

SetBuff:
	LOG(Warning, TEXT("zebra buff"));
	// speed up for 5 second
	GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()->void {
		GetCharacterMovement()->MaxWalkSpeed = CharacterMaxSpeed;
		bOnZebraBuff = false;
		LOG(Warning, TEXT("zebra buff end"));
		}), 5.0f, false);
}

void AEcoActorCharacter::ElephantBuff()
{
	// heal
	LOG(Warning, TEXT("heal"));
	CharacterStat->SetHP(CharacterStat->GetCurrentHP() + 30.0f);
}

void AEcoActorCharacter::CrocoBuff()
{
	FTimerHandle TimerHandle;
	if (bOnCrocoBuff)
	{
		HitDamage = ConstHitDam; ShotDamage = ConstShotDam;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		goto SetBuff;
	}

	bOnCrocoBuff = true;

SetBuff:
	LOG(Warning, TEXT("croco buff start"));
	// extra damage for 5 second
	HitDamage *= 2.0f; ShotDamage *= 2.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()->void {
		HitDamage = ConstHitDam;
		ShotDamage = ConstShotDam;
		bOnCrocoBuff = false;
		LOG(Warning, TEXT("croco buff end"));
		}), 5.0f, false);
}

void AEcoActorCharacter::Complete()
{
	LOG(Warning, TEXT("success"));
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SuccessWidget = CreateWidget(GetWorld(), SuccessWidgetClass);
	bCanBeDamaged = false;
	if (IsValid(SuccessWidget))
	{
		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		SuccessWidget->AddToViewport();
		
		GetMovementComponent()->Deactivate();

		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}