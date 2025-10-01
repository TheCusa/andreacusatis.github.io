// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "LaserMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API ULaserMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULaserMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Movement(float);
	UPROPERTY(EditAnywhere)
	FVector Direction;

	void SetDirection(FVector);
	
	UPROPERTY(EditAnywhere)
	float Speed;

	void SetSpeed(float);

private:

	UPROPERTY(VisibleAnywhere)
	FVector StartPoint;

	// TargetLocation is calculated Server side. Used by clients to interpolate to such location
	UPROPERTY(ReplicatedUsing=OnRep_TargetLocation, VisibleAnywhere)
	FVector TargetLocation;

	UFUNCTION()
	void OnRep_TargetLocation();
	
};
