// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame/Core/Puzzle/LaserEndPoint.h"

// Sets default values
ALaserEndPoint::ALaserEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaserEndPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

