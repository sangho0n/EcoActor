// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"
#include "HunterAiController.h"
#include "Kismet/GameplayStatics.h"
#include "HunterHPWidget.h"
#include "Components/WidgetComponent.h"
#include "Camera/PlayerCameraManager.h"
//#include "DrawDebugHelpers.h"

// Sets default values
AHunter::AHunter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Hunter"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));

	// skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);

	// 여자 : /Game/MilitaryCharDark/MW_Style2_Female.MW_Style2_Female
	// 남자 : /Game/MilitaryCharDark/MW_Style2_Male.MW_Style2_Male
	bool GetMaleAsset = FMath::RandBool();
	if (GetMaleAsset)
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HUNTER(TEXT("/Game/MilitaryCharDark/MW_Style2_Male.MW_Style2_Male"));
		if (SK_HUNTER.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_HUNTER.Object);
		}
	}
	else
	{
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HUNTER(TEXT("/Game/MilitaryCharDark/MW_Style2_Female.MW_Style2_Female"));
		if (SK_HUNTER.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(SK_HUNTER.Object);
		}
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIMINS_HUNTER(TEXT("/Game/Main/Anim/Hunter/BP_HunterAnim.BP_HunterAnim_C"));
	if (ANIMINS_HUNTER.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMINS_HUNTER.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBARWIDGET(TEXT("/Game/Main/UI/HunterHPBar.HunterHPBar_C"));
	if (HPBARWIDGET.Succeeded())
	{
		HPBarWidget->SetWidgetClass(HPBARWIDGET.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}


	CharacterStat = CreateDefaultSubobject<UCharacterStat>(TEXT("CharacterState"));
	CharacterStat->SetMaxHP(50.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed /= 2.0f;

	AIControllerClass = AHunterAiController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AttackRange = 150.0f;
	AttackRadius = 50.0f;
	HunterAttackDamage = 10.0f;

	bCanBeDamaged = true;
	DeadSecCount = 3;
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();
	bAttacked = false;

	auto HPWidget = Cast<UHunterHPWidget>(HPBarWidget->GetUserWidgetObject());
	if (HPWidget->IsValidLowLevel())
	{
		HPWidget->BindHunterStat(CharacterStat);
	}

	auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->OnCharacterStateChanged.AddDynamic(this, &AHunter::SetHunterState);


	SetHunterState(ECharacterState::READY);

	//Player->CharacterStat->OnScoreReachTop.AddDynamic(this, &AHunter::SetDead);
}

void AHunter::SetHunterState(ECharacterState NewState)
{
	if (NewState == ECharacterState::DEAD) return;

	HunterState = NewState;

	switch (HunterState) 
	{
	case ECharacterState::PREINIT:
	{
		bCanBeDamaged = false;
		bIsDead = false;

		break;
	}
	case ECharacterState::LOADING:
	{
		bCanBeDamaged = false;
		bIsDead = false;
		HPBarWidget->SetVisibility(false);
		SetActorHiddenInGame(true);
		Cast<AHunterAiController>(GetController())->StopAI();

		break;
	}
	case ECharacterState::READY:
	{
		bCanBeDamaged = true;
		bIsDead = false;
		HPBarWidget->SetVisibility(false);
		SetActorHiddenInGame(false);
		Cast<AHunterAiController>(GetController())->RunAI();

		break;
	}
	}
}

void AHunter::SetHPBarVisible()
{
	if (bAttacked) return; //already attacked

	bAttacked = true;
	HPBarWidget->SetVisibility(true);
}

void AHunter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UHunterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnHunterAttack.AddDynamic(this, &AHunter::Attack);

	CharacterStat->OnHPIsZero.AddDynamic(this, &AHunter::SetDead);

	CharacterStat->OnHPChanged.AddLambda([this]()->void {
		});
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// make hpbar widget to see screen
	if (bAttacked)
	{
		auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		auto CameraLoc = Player->GetCameraComponent()->GetComponentLocation();
		auto HPBarLoc = HPBarWidget->GetComponentLocation();
		auto HPBarToCameraRot = (CameraLoc - HPBarLoc).Rotation();

		HPBarWidget->SetWorldRotation(HPBarToCameraRot);
	}
}


float AHunter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (!bCanBeDamaged) return 0.0f;
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FinalDamage = DamageAmount;
	CharacterStat->SetDamage(FinalDamage);

	LOG(Warning, TEXT("hunter HP %f"), CharacterStat->GetCurrentHP());

	return FinalDamage;
}


void AHunter::Attack()
{
	FVector ForwardVec = GetActorForwardVector();
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + ForwardVec * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(HunterAttackDamage, DamageEvent, this->GetController(), this);
	}

//#if ENABLE_DRAW_DEBUG
//	FVector TraceVec = ForwardVec * AttackRange;
//
//	if (bResult)
//	{
//		DrawDebugCapsule(
//			GetWorld(),
//			GetActorLocation() + TraceVec * 0.5f,
//			TraceVec.Size() * 0.5f,
//			AttackRadius,
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
//			AttackRadius,
//			FRotationMatrix::MakeFromZ(TraceVec).ToQuat(),
//			FColor::Green,
//			false,
//			3.0f
//		);
//
//	}
//#endif
}


void AHunter::SetDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
	bIsDead = true;
	bCanBeDamaged = false;
	GetController()->Destroy();
	AnimInstance->PlayDeadAnim();
	HPBarWidget->SetVisibility(false);

	auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterStat->ScoreUp();

	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), DeadSecCount, false);
}
