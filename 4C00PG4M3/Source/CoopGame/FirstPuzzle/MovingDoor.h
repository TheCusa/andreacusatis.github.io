// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "MovingDoor.generated.h"

UENUM(BlueprintType)
enum class EDoorOpenAxis : uint8
{
	X UMETA(DisplayName="X Axis"),
	Y UMETA(DisplayName="Y Axis"),
	Z UMETA(DisplayName="Z Axis")
};

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	Door UMETA(DisplayName="Door"),
	Gate UMETA(DisplayName="Blocking Gate"),
};

UCLASS()
class COOPGAME_API AMovingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingDoor();
	
	// Close and open door funtions
	void CloseDoor();
	void OpenDoor();

	bool IsOpen();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static mesh reference. Assignable in BP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh Component")
	UStaticMeshComponent* DoorMesh;

	// Curve for animation. Assignable from editor
	UPROPERTY(EditAnywhere, Category = "Door animation curve")
	UCurveFloat* SlideCurve;

	UPROPERTY(EditAnywhere, Category = "Door movement distance vector")
	FVector SlideMovementVector = FVector(149.0f, 0.f, 0.0f);

	// Timeline
	FTimeline DoorTimeline;

	// Speed of the door opening/closing animation
	UPROPERTY(EditAnywhere, Category = "Door animation")
	float DoorOpenSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Door animation")
	bool FixedOffsetMovement = true;

	UPROPERTY(EditAnywhere, Category = "Door animation")
	bool ReverseMovement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door animation")
	EDoorOpenAxis OpenAxis = EDoorOpenAxis::X;

	UPROPERTY(EditDefaultsOnly, Category = "Door animation")
	EDoorType DoorType = EDoorType::Door;

	UFUNCTION()
	void HandleProgress(float Value);

private:

	UFUNCTION()
	void HandleAlarmChanged(bool bNewState);

	UPROPERTY(Replicated, EditAnywhere, Category = "Door animation")
	bool bIsOpen;		// Open/Close door status

	// Door Locations
	FVector ClosedLocation;
	FVector OpenLocation;
};