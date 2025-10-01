// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserSpawner.h"
#include "Components/BoxComponent.h"
#include "LaserHandler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "CoopGame/Core/Components/LaserMovementComponent.h"

// Sets default values
ALaserSpawner::ALaserSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxRef = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxRef;
}

// Called when the game starts or when spawned
void ALaserSpawner::BeginPlay()
{
	Super::BeginPlay();
	//Spawning a temp laser to get actor bounds
	ALaserHandler* TempLaser = GetWorld()->SpawnActor<ALaserHandler>(LaserClass);
	if (TempLaser)
	{
		TempLaser->GetActorBounds(false, LaserOrigin, LaserBoxExtent);
		TempLaser->Destroy();
	}
	SpawnCalculator();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&ALaserSpawner::SpawnLaser,
			LaserTimer,
			true);
	}
}

void ALaserSpawner::SpawnCalculator()
{
	//Calculating widhts and bounds
	GetActorBounds(false, SpawnerOrigin, SpawnerBoxExtent);
	float SpawnerWidth;
	float LaserWidth;
	if (SpawnerBoxExtent.X > SpawnerBoxExtent.Y)
	{
		SpawnerWidth = SpawnerBoxExtent.X * 2.0f;

		SpawnParams.Owner = this;

		LaserWidth = LaserBoxExtent.X * 2.0f;

		LaserOffset = FVector(LaserWidth, 0.f, 0.f); // LaserOffset is the FVector built from BoxExtent, used for the offset between lasers

		LaserNumber = SpawnerWidth / (LaserWidth + Offset.X);
	}
	else
	{
		SpawnerWidth = SpawnerBoxExtent.Y * 2.0f;

		SpawnParams.Owner = this;

		LaserWidth = LaserBoxExtent.Y * 2.0f;

		LaserOffset = FVector(0.f, LaserWidth, 0.f);

		LaserNumber = SpawnerWidth / (LaserWidth + Offset.Y);
	}
}

void ALaserSpawner::SpawnLaser()
{
	if (ActorHasTag("horizontal") && SingleLaser)
	{
		LaserNumber = 1;
		FVector SpawnLocation = SpawnerOrigin;
		ALaserHandler* LaserRef;
		LaserRef = GetWorld()->SpawnActor<ALaserHandler>(SingleLaser, SpawnLocation, LaserRotation, SpawnParams);
		LaserRef->FindComponentByClass<ULaserMovementComponent>()->SetDirection(LaserDirection);
		LaserRef->FindComponentByClass<ULaserMovementComponent>()->SetSpeed(Speed);
	}
	else {
		int SafeLaserNumber = FMath::RandRange(0, LaserNumber - 1);
		FVector SpawnLocation = (SpawnerOrigin - SpawnerBoxExtent) + SpawnOffset; //Setting initial spawn location, SpawnOffset is offset from borders

		for (int i = 0; i < LaserNumber; ++i)
		{
			if (LaserClass && FakeLaserClass)
			{
				ALaserHandler* LaserRef;
				if (SpawnerBoxExtent.Y > SpawnerBoxExtent.X)
				{
					SpawnRotation = FRotator(0, 90, 0);
				}
				if (i == SafeLaserNumber)
				{
					LaserRef = GetWorld()->SpawnActor<ALaserHandler>(FakeLaserClass, SpawnLocation, SpawnRotation, SpawnParams);

					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(SafeLaserNumber)); //Debug purpose
				}
				else
				{
					LaserRef = GetWorld()->SpawnActor<ALaserHandler>(LaserClass, SpawnLocation, SpawnRotation, SpawnParams);
				}
				LaserRef->FindComponentByClass<ULaserMovementComponent>()->SetDirection(LaserDirection);
				LaserRef->FindComponentByClass<ULaserMovementComponent>()->SetSpeed(Speed);
				SpawnLocation += LaserOffset + Offset;
			}
		}
	}
}
