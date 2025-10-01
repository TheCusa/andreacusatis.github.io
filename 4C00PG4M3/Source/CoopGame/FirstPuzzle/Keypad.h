// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObjectInterface.h"
#include "MovingDoor.h"
#include "Keypad.generated.h"

class UBoxComponent;
class AAgentPlayerController;
class UWidgetComponent;

UCLASS()
class COOPGAME_API AKeypad : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeypad();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Inherited functions from IInteractableObjectInterface
	UFUNCTION(BlueprintCallable)
	virtual void ExecuteAction() override;

	// RPC call for check code solution
	UFUNCTION(Server, Reliable)
	void Server_CheckCode(const TArray<int8>& EnteredCode);

	// RPC call for save code into gamestate
	UFUNCTION(Server, Reliable)
	void Server_SaveCodeIntoGameState();


	// Collider reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collider")
	UBoxComponent* KeypadInteractionCollider;

	// Static mesh reference. Assignable in BP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* KeypadMesh;

	// UI Keypad Widget Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* KeypadWidgetComponent;

	// Target actor reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TARGET DOOR REFERENCE")
	AMovingDoor* TargetDoor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap begin function for interaction box collider. Inherited functions from IInteractableObjectInterface
	UFUNCTION()
	virtual void OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Overlap end function for interaction box collider. Inherited functions from IInteractableObjectInterface
	UFUNCTION()
	virtual void OnInteractionBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

private:
	//UUserWidget* KeypadUIWidget;
	TArray<int8> Code;
	AAgentPlayerController* AgentPC;

	void ShowUIChangeInputMode();
	void HideUIChangeInputMode();
	void GenerateNewCode();
	void OpenDoor();

	UFUNCTION()
	void HandleWidgetCodeSent(const TArray<int8>& SentCode);
};