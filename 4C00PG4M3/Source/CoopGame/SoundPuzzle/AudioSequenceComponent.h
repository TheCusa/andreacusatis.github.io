// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioSequenceComponent.generated.h"

class USoundBase;
class USoundDataAsset;

UCLASS()
class COOPGAME_API UAudioSequenceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAudioSequenceComponent();

	UFUNCTION()
	void PlayNoteSequence(const TArray<int8>& Notes);

	UFUNCTION()
	void StopSequence();

	UFUNCTION()
	void OnNoteFinished();

protected:
	virtual void BeginPlay() override;

	void PlayCurrentNote();

	UPROPERTY()
	TObjectPtr<UAudioComponent> SequenceAudioComponent;

	// Sequence data
	UPROPERTY()
	TArray<int8> PendingNotes;

	UPROPERTY()
	int32 CurrentNoteIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundDataAsset> SoundDataAsset;
	
	bool bIsPlaying;
};
