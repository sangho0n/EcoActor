// Fill out your copyright notice in the Description page of Project Settings.


#include "Hunter.h"
#include "HunterAiController.h"
#include "HunterAnimInstance.h"
#include "Kismet/GameplayStatics.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed /= 2.0f;

	AIControllerClass = AHunterAiController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bAttacked = false;
}

// Called when the game starts or when spawned
void AHunter::BeginPlay()
{
	Super::BeginPlay();

}

void AHunter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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