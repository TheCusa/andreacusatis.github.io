// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"


class USphereComponent;
class ABoard;
class ARespawnHandler;
UCLASS()
class MARBLEMAZE_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void Tick(float) override;

	FVector StartingSpawn; 
private:
	 float MinZ = -2000;
	 float MaxZ;
	 float Offset;

	 UPROPERTY()
	 ABoard* BoardRef;
	 UPROPERTY()
	 ARespawnHandler* RespawnHandlerRef;
};
