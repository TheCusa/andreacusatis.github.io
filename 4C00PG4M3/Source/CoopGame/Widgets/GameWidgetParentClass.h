// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputSubsystems.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidgetParentClass.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API UGameWidgetParentClass : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	virtual void OnLeftKeyPressed(){}
	UFUNCTION()
	virtual void OnRightKeyPressed(){}
	UFUNCTION()
	virtual void OnUpArrowPressed(){}
	UFUNCTION()
	virtual void OnDownArrowPressed(){}
	UFUNCTION()
	virtual void OnEnterPressed(){}
	UFUNCTION()
	virtual void OnCancelPressed(){}
	
};
