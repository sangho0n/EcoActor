// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "Hunter.h"
#include "EcoActorCrocodile.h"
#include "EcoActorElephant.h"
#include "EcoActorZebra.h"
#include "Gun.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	auto capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = capsule;
	capsule->InitCapsuleSize(0.0f, 0.0f);
	capsule->SetCollisionProfileName(TEXT("NoCollision"));

	HunterTime = 3.0f;
	AnimalTime = 3.0f;
	GunTime = 5.0f;
	MaxHunter = 8;
	MaxAnimal = 5;
	MaxGun = 3;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	CurrHunter = 0;
	CurrAnimal = 0;
	CurrGun = 0;
	SpawnHunter();
	SpawnAnimal();
	SpawnGun();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::SpawnHunter()
{
	GetWorld()->GetTimerManager().SetTimer(HunterHandle, FTimerDelegate::CreateLambda([&]()->void {
		if (CurrHunter >= MaxHunter) return;

	ReRand:
		FVector2D RandomPoint = FMath::RandPointInCircle(2000.0f);
		RandomPoint.X += GetActorLocation().X;
		RandomPoint.Y += GetActorLocation().Y;

		// 지면으로 linetrace
		FHitResult HitResult;
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		bool bResult = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			FVector(RandomPoint, 1000.0f),
			FVector(RandomPoint, -1000.0f),
			ECollisionChannel::ECC_GameTraceChannel7,
			Params
		);

		// 유효한 위치일 경우 spawn
		// 유효하지 않은 위치일 경우 다시 randompoint 얻기
		if (!bResult) goto ReRand;
		if (nullptr == GetWorld()->SpawnActor<AHunter>(HitResult.Location, FRotator::ZeroRotator)) { goto ReRand; }

		CurrHunter++;
		}), HunterTime, true);
}

void ASpawner::SpawnAnimal()
{
	GetWorld()->GetTimerManager().SetTimer(AnimalHandle, FTimerDelegate::CreateLambda([&]()->void {
		if (CurrAnimal >= MaxAnimal) return;

	ReRand:
		FVector2D RandomPoint = FMath::RandPointInCircle(2000.0f);
		RandomPoint.X += GetActorLocation().X;
		RandomPoint.Y += GetActorLocation().Y;

		int8 RandomAnimal = FMath::RandRange(1, 3);
		// 지면으로 linetrace
		FHitResult HitResult;
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		bool bResult = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			FVector(RandomPoint, 1000.0f),
			FVector(RandomPoint, -1000.0f),
			ECollisionChannel::ECC_GameTraceChannel7,
			Params
		);

		// 유효한 위치일 경우 spawn
		// 유효하지 않은 위치일 경우 다시 randompoint 얻기
		if (!bResult) goto ReRand;
		switch (RandomAnimal)
		{
		case 3:
		{
			if (nullptr == GetWorld()->SpawnActor<AEcoActorZebra>(HitResult.Location, FRotator::ZeroRotator)) { goto ReRand; }
			break;
		}
		case 2:
		{
			if (nullptr == GetWorld()->SpawnActor<AEcoActorElephant>(HitResult.Location, FRotator::ZeroRotator)) { goto ReRand; }
			break;
		}
		case 1:
		{
			if (nullptr == GetWorld()->SpawnActor<AEcoActorCrocodile>(HitResult.Location, FRotator::ZeroRotator)) { goto ReRand; }
			break;
		}
		}

		CurrAnimal++;
		}), AnimalTime, true);
}

void ASpawner::SpawnGun()
{
	GetWorld()->GetTimerManager().SetTimer(GunHandle, FTimerDelegate::CreateLambda([&]()->void {
		if (CurrGun >= MaxGun) return;
	ReRand:
		FVector2D RandomPoint = FMath::RandPointInCircle(2000.0f);
		RandomPoint.X += GetActorLocation().X;
		RandomPoint.Y += GetActorLocation().Y;

		// 지면으로 linetrace
		FHitResult HitResult;
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		bool bResult = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			FVector(RandomPoint, 1000.0f),
			FVector(RandomPoint, -1000.0f),
			ECollisionChannel::ECC_GameTraceChannel7,
			Params
		);

		// 유효한 위치일 경우 spawn
		// 유효하지 않은 위치일 경우 다시 randompoint 얻기
		if (!bResult) goto ReRand;
		if (nullptr == GetWorld()->SpawnActor<AGun>(HitResult.Location, FRotator::ZeroRotator)) { goto ReRand; }

		CurrGun++;
		}), GunTime, true);
}

void ASpawner::DecCurrHunter()
{
	CurrHunter--;
}

void ASpawner::DecCurrAnimal()
{
	CurrAnimal--;
}
void ASpawner::DecCurrGun()
{
	CurrGun--;
}