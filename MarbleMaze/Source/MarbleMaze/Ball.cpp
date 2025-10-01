// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Board.h"
#include "Components/SphereComponent.h"  
#include "Kismet/GameplayStatics.h"
#include "RespawnHandler.h"
// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	StartingSpawn = GetActorLocation();
    BoardRef = Cast<ABoard>(UGameplayStatics::GetActorOfClass(this, ABoard::StaticClass()));
    if (BoardRef)
    {
        float HalfSide;
        FVector Origin;
        FVector BoxExtent;

        UStaticMeshComponent* BoardMeshRef = BoardRef->GetComponentByClass<UStaticMeshComponent>();
        BoardMeshRef->GetLocalBounds(Origin, BoxExtent);
        HalfSide = BoxExtent.X;

        Offset = FMath::Tan(FMath::DegreesToRadians(BoardRef->MaxAngle)) * (2.f *HalfSide);

        MaxZ = BoardRef->GetActorLocation().Z + Offset;

    }
    RespawnHandlerRef = Cast<ARespawnHandler>(UGameplayStatics::GetActorOfClass(this, ARespawnHandler::StaticClass()));
    if (RespawnHandlerRef)
    {
        MinZ = RespawnHandlerRef->GetActorLocation().Z;
    }




}
void ABall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Location = GetActorLocation();

    // Clamp della Z
    Location.Z = FMath::Clamp(Location.Z, MinZ, MaxZ);

    SetActorLocation(Location);
}

