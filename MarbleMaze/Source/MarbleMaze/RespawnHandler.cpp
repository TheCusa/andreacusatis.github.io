// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnHandler.h"
#include "Components/BoxComponent.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARespawnHandler::ARespawnHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    RootComponent = BoxCollision;
    
}

// Called when the game starts or when spawned
void ARespawnHandler::BeginPlay()
{
	Super::BeginPlay();
    
    BallRef = Cast<ABall>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall::StaticClass()));
    if (BoxCollision)
    {
        BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARespawnHandler::OverlapHandler);
    }
}

void ARespawnHandler::OverlapHandler(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor->IsA(ABall::StaticClass()))
    {
        BallRef->SetActorLocation(BallRef->StartingSpawn);
    }
}


