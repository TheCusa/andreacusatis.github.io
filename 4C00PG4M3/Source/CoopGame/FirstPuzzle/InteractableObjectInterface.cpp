// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObjectInterface.h"

// Add default functionality here for any IInteractableObjectInterface functions that are not pure virtual.
void  IInteractableObjectInterface::ExecuteAction()
{
	return;
}

// Get text for the UI prompt function
FText IInteractableObjectInterface::GetUIPromptText() const
{
	return FText::FromString("Press E");
}

void IInteractableObjectInterface::OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	return;
}

void IInteractableObjectInterface::OnInteractionBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	return;
}