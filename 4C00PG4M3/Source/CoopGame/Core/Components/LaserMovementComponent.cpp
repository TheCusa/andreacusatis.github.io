// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserMovementComponent.h"

// Sets default values for this component's properties
ULaserMovementComponent::ULaserMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void ULaserMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetOwner()->GetActorLocation();
	GetOwner()->SetActorLocation(StartPoint);
	// ...
	
}


// Called every frame
void ULaserMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Server moves the Laser
	if (GetOwner()->HasAuthority())
	{
		Movement(DeltaTime);
	}
	
	else //Clients only. Interpolate to TargetLocation manually
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector SmoothedLoc = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.f);
		GetOwner()->SetActorLocation(SmoothedLoc);
	}
}

void ULaserMovementComponent::Movement(float DeltaTime)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	CurrentLocation += Speed * DeltaTime * Direction;
	GetOwner()->SetActorLocation(CurrentLocation);

	TargetLocation = CurrentLocation; // replicate to clients
}

// Fallback function to replication. no actions needed
void ULaserMovementComponent::OnRep_TargetLocation()
{
}

void ULaserMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULaserMovementComponent, TargetLocation);
}

void ULaserMovementComponent::SetDirection(FVector NewDir)
{
	Direction = NewDir;
}

void ULaserMovementComponent::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
}