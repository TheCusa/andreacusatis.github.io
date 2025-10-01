// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnHandler.generated.h"

class UBoxComponent;
class ABall;
UCLASS()
class MARBLEMAZE_API ARespawnHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
private:
	UFUNCTION()
	void OverlapHandler(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UPROPERTY()
	ABall* BallRef;
};
