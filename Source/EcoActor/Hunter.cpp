// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"
#include "HunterAiController.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

// Sets default values
AHunter::AHunter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Hunter"));

	// skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

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

	CharacterStat = CreateDefaultSubobject<UCharacterStat>(TEXT("CharacterState"));
	CharacterStat->SetMaxHP(50.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed /= 2.0f;

	AIControllerClass = AHunterAiController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bAttacked = false;
	AttackRange = 150.0f;
	AttackRadius = 50.0f;
	HunterAttackDamage = 10.0f;
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();

}

void AHunter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UHunterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnHunterAttack.AddDynamic(this, &AHunter::Attack);
}

// Called every frame
void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AHunter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	LOG(Warning, TEXT("Hunter Took Damget : %f"), DamageAmount);

	return FinalDamage;
}

void AHunter::SetPlayer()
{
	Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Player))
	{
		Player->OnValidAttack.AddLambda([this]() -> void {

			});
	}
}

bool AHunter::GetAttacked()
{
	return bAttacked;
}

void AHunter::SetAttacked()
{
	bAttacked = true;
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