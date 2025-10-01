// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Board.generated.h"

class UCameraComponent;
class UBoxComponent;
class ABall;
class AMarbleGameMode;
UCLASS()
class MARBLEMAZE_API ABoard : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector BoxCollisionLocation;
private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	UPROPERTY()
	ABall* BallRef;
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UPROPERTY()
	AMarbleGameMode* GameModeRef;
	UPROPERTY(EditAnywhere, Category = "Input")
	float MouseSensitivity = 2.5f;

	void TiltForward(float Value);   // Pitch
	void TiltLateral(float Value);   // Roll

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerExit(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	void QuitGame();
public:
	float MaxAngle = 5.f;

};
