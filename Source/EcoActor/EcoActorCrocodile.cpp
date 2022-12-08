// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorCrocodile.h"
#include "AnimalAIController.h"
#include "AnimalAnimInstance.h"
#include "EcoActorCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEcoActorCrocodile::AEcoActorCrocodile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 60.0f);
	GetCapsuleComponent()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Animal"));

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -60.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CROCO(TEXT("/Game/AfricanAnimalsPack/Crocodile/Meshes/SK_Crocodile.SK_Crocodile"));
	if (SK_CROCO.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CROCO.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CROCOANIM(TEXT("/Game/Main/Anim/Animals/croco/BP_CrocoAnim.BP_CrocoAnim_C"));
	if (CROCOANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CROCOANIM.Class);
	}
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	AIControllerClass = AAnimalAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEcoActorCrocodile::BeginPlay()
{
	Super::BeginPlay();
	
}


void AEcoActorCrocodile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEcoActorCrocodile::OnCharacterBeginOverlap);
}

// Called every frame
void AEcoActorCrocodile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEcoActorCrocodile::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<AEcoActorCharacter>(OtherActor);
	Player->CrocoBuff();
	Destroy();
}