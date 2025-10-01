// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioSequenceComponent.h"

#include "SoundDataAsset.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"

UAudioSequenceComponent::UAudioSequenceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentNoteIndex = 0;
    bIsPlaying = false;
}

void UAudioSequenceComponent::BeginPlay()
{
    Super::BeginPlay();
    
    SequenceAudioComponent = NewObject<UAudioComponent>(GetOwner());
    SequenceAudioComponent->SetupAttachment(GetOwner()->GetRootComponent());
    
    SequenceAudioComponent->OnAudioFinished.AddDynamic(this, &UAudioSequenceComponent::OnNoteFinished);
    SequenceAudioComponent->RegisterComponent();
}

void UAudioSequenceComponent::PlayNoteSequence(const TArray<int8>& Notes)
{
    if (bIsPlaying)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sequence already playing, stopping current"));
        StopSequence();
    }
    
    if (Notes.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid SoundAsset or empty Notes array"));
        return;
    }
    
    PendingNotes = Notes;
    CurrentNoteIndex = 0;
    bIsPlaying = true;
    
    UE_LOG(LogTemp, Display, TEXT("Starting audio sequence with %d notes"), Notes.Num());
    
    PlayCurrentNote();
}

void UAudioSequenceComponent::PlayCurrentNote()
{
    if (!bIsPlaying || CurrentNoteIndex >= PendingNotes.Num())
    {
        bIsPlaying = false;
        UE_LOG(LogTemp, Display, TEXT("Audio sequence completed!"));
        return;
    }
    
    if (!SoundDataAsset || !SequenceAudioComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing SoundDataAsset or AudioComponent"));
        return;
    }
    
    TObjectPtr<USoundBase> SoundBase = SoundDataAsset->GetKeySound(static_cast<Notes>(PendingNotes[CurrentNoteIndex]));
    
    if (!SoundBase)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get sound for note %d"), PendingNotes[CurrentNoteIndex]);
        CurrentNoteIndex++;
        PlayCurrentNote();
        return;
    }
    
    SequenceAudioComponent->SetSound(SoundBase);
    SequenceAudioComponent->SetWorldLocation(GetOwner()->GetActorLocation());
    SequenceAudioComponent->Play();
    SequenceAudioComponent->FadeOut(0.5f,0.2f);
    
    UE_LOG(LogTemp, Display, TEXT("Playing note %d/%d: %s"), 
           CurrentNoteIndex + 1, PendingNotes.Num(), *SoundBase->GetName());
}

void UAudioSequenceComponent::OnNoteFinished()
{
    if (bIsPlaying)
    {
        CurrentNoteIndex++;
        PlayCurrentNote();
    }
}

void UAudioSequenceComponent::StopSequence()
{
    bIsPlaying = false;
    CurrentNoteIndex = 0;
    
    if (SequenceAudioComponent && SequenceAudioComponent->IsPlaying())
    {
        SequenceAudioComponent->Stop();
    }
    UE_LOG(LogTemp, Display, TEXT("Audio sequence stopped"));
}
