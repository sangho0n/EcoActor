// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "EcoActorCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));

	Trigger->SetBoxExtent(FVector(30.0f, 60.0f, 60.0f));
	Trigger->SetRelativeLocation(FVector(0.0f, 20.0f, 0.0f));
	RootComponent = Trigger;
	Gun->SetupAttachment(RootComponent);

	Gun->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RIFLE(TEXT("/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B.Assault_Rifle_B"));
	if (RIFLE.Succeeded())
	{
		Gun->SetSkeletalMesh(RIFLE.Object);
	}

	Trigger->SetCollisionProfileName(TEXT("Weapon"));
	Gun->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGun::OnCharacterBeginOverlap);
}

void AGun::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LOG(Warning, TEXT("GUN"));
	auto Character = Cast<AEcoActorCharacter>(OtherActor);

	if (Character->isAttacking()) return;

	Character->Equip();

	Destroy();
}
