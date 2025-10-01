// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterDefinition.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API UCharacterDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FName CharacterName;

	UPROPERTY(EditDefaultsOnly)
	UTexture* Icon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimationBlueprint;

	USkeletalMesh* GetMesh() const;
};
