// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HackerMonitor.generated.h"

class UAudioSequenceComponent;
class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class COOPGAME_API AHackerMonitor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHackerMonitor();
	
	UFUNCTION()
	void TriggerAudioSequenceForClient(APlayerController* TargetClient, const TArray<int8>& Notes);

	UFUNCTION(Client, Reliable)
	void ClientPlayAudioSequence(const TArray<int8>& Notes);
	
	UFUNCTION()
	void StopAudioSequence() const;

protected:
	// Static Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// Widget Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioSequenceComponent* AudioSequenceComponent;
	
};
