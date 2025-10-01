// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserHandler.generated.h"


class UBoxComponent;
class ARespawnPoint;
UCLASS()
class COOPGAME_API ALaserHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Laser")
	UStaticMeshComponent* LaserMesh;
	
private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;

	UPROPERTY()
	ARespawnPoint* RespawnPointRef;

	UPROPERTY(EditAnywhere)
	bool bIsReal;

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void HandleCollision(AActor*);
};
