// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorElephant.h"
#include "AnimalAIController.h"
#include "AnimalAnimInstance.h"
#include "EcoActorCharacter.h"
#include "Spawner.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEcoActorElephant::AEcoActorElephant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(180.f, 180.0f);
	GetCapsuleComponent()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f, 0.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Animal"));

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -160.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELEPHANT(TEXT("/Game/AfricanAnimalsPack/Elephant/Meshes/SK_Elephant.SK_Elephant"));
	if (SK_ELEPHANT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ELEPHANT.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ELEPHANTANIM(TEXT("/Game/Main/Anim/Animals/elephant/BP_ElephAnim.BP_ElephAnim_C"));
	if (ELEPHANTANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ELEPHANTANIM.Class);
	}
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	AIControllerClass = AAnimalAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEcoActorElephant::BeginPlay()
{
	Super::BeginPlay();
}

void AEcoActorElephant::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEcoActorElephant::OnCharacterBeginOverlap);
}

// Called every frame
void AEcoActorElephant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEcoActorElephant::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEcoActorCharacter::StaticClass()))
	{
		auto Player = Cast<AEcoActorCharacter>(OtherActor);
		Player->ElephantBuff();
	}
	auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->GetSpawner()->DecCurrAnimal();
	Destroy();
}