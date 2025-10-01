// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame/Core/Puzzle/HackerMonitor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "CoopGame/SoundPuzzle/AudioSequenceComponent.h"


// Sets default values
AHackerMonitor::AHackerMonitor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    // Create and setup Static Mesh Component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Create and setup Widget Component
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(MeshComponent);

    // Configure widget settings
    WidgetComponent->SetDrawAtDesiredSize(true);
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	// Create Audio Sequence Component
	AudioSequenceComponent = CreateDefaultSubobject<UAudioSequenceComponent>(TEXT("AudioSequenceComponent"));	
}

void AHackerMonitor::TriggerAudioSequenceForClient(APlayerController* TargetClient, const TArray<int8>& Notes)
{
	if (!TargetClient)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid TargetClient"));
		return;
	}

	// RPC
	ClientPlayAudioSequence(Notes);
	UE_LOG(LogTemp, Display, TEXT("Triggering audio sequence for client: %s"), *TargetClient->GetName());
}

void AHackerMonitor::ClientPlayAudioSequence_Implementation(const TArray<int8>& Notes)
{
	if (!AudioSequenceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AudioSequenceComponent is null on client!"));
		return;
	}
    
	UE_LOG(LogTemp, Display, TEXT("Playing audio sequence on client"));
	AudioSequenceComponent->PlayNoteSequence(Notes);
}

void AHackerMonitor::StopAudioSequence() const
{
	if (AudioSequenceComponent)
	{
		AudioSequenceComponent->StopSequence();
	}
}


