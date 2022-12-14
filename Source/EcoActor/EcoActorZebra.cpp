// Fill out your copyright notice in the Description page of Project Settings.


#include "EcoActorZebra.h"
#include "AnimalAIController.h"
#include "AnimalAnimInstance.h"
#include "EcoActorCharacter.h"
#include "spawner.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEcoActorZebra::AEcoActorZebra()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(90.0f, 100.0f);
	GetCapsuleComponent()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f, 0.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Animal"));

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName("NoCollision");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ZEBRA(TEXT("/Game/AfricanAnimalsPack/Zebra/Meshes/SK_Zebra.SK_Zebra"));
	if (SK_ZEBRA.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ZEBRA.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZEBRAANIM(TEXT("/Game/Main/Anim/Animals/Zebra/BP_ZebraAnim.BP_ZebraAnim_C"));
	if (ZEBRAANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ZEBRAANIM.Class);
	}
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	AIControllerClass = AAnimalAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEcoActorZebra::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEcoActorZebra::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEcoActorZebra::OnCharacterBeginOverlap);
}

// Called every frame
void AEcoActorZebra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEcoActorZebra::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEcoActorCharacter::StaticClass()))
	{
		auto Player = Cast<AEcoActorCharacter>(OtherActor);
		Player->ZebraBuff();
	}
	auto Player = Cast<AEcoActorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->GetSpawner()->DecCurrAnimal();
	Destroy();
}