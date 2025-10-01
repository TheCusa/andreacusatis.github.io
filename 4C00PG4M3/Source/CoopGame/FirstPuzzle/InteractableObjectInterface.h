// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObjectInterface.generated.h"

/*
*	Empty class for reflection system visibility.
*	Uses the UINTERFACE macro. 
*	Inherits from UInterface.
*/
UINTERFACE(MinimalAPI)
class UInteractableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
class COOPGAME_API IInteractableObjectInterface
{
	GENERATED_BODY()

public:
	// Function that implement the execution of an action whe something interact with this object. Must be ovveride in the derived class.
	virtual void ExecuteAction();

	// Get text for the UI prompt function
	virtual FText GetUIPromptText() const;

	virtual void OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnInteractionBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
