// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoopGame/Characters/CharacterParentClass.h"
#include "AgentCharacter.generated.h"

UCLASS()
class AAgentCharacter : public ACharacterParentClass
{
	GENERATED_BODY()

public:
	AAgentCharacter();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

};