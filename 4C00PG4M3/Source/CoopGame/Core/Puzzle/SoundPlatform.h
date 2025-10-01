// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame/SoundPuzzle/SoundDataAsset.h"
#include "SoundPlatform.generated.h"

class UBoxComponent;
class AAgentPlayerController;
class APiano;
class ACoopGameState;
UCLASS()
class COOPGAME_API ASoundPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
private:
	UPROPERTY()
	USoundBase* Note = nullptr;
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundDataAsset* SoundAsset;
	UPROPERTY()
	AAgentPlayerController* PCRef;

	UPROPERTY()
	APiano* PianoRef;

	UPROPERTY(EditAnywhere, Category = "Sound")
	Notes NoteName;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	void PlayKeySound(Notes );
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySound();
};
