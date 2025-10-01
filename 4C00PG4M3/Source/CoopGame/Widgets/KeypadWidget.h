// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Delegates/Delegate.h"
#include "KeypadWidget.generated.h"


class UTextBlock;
class UButton;


// Create a delegate function to execute somewhere when SendCode() function is called
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCodeEntered, const TArray<int8>&, EnteredCode);

UCLASS(Blueprintable)
class COOPGAME_API UKeypadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCodeEntered OnCodeEntered;	// Delegate event variable

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FirstSlotText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SecondSlotText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ThirdSlotText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FourthSlotText;

	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton1;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton2;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton3;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton4;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton5;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton6;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton7;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton8;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton9;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButton0;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButtonCancel;
	UPROPERTY(meta = (BindWidget))
	UButton* KeyButtonEnter;

	int MaxCodeNumbers;
	TArray <int8> InputCode;


	void ButtonClickedHandler(const int32);
	void AddCodeNumber(const int32);
	void DisplayOnScreen(int32);
	void ClearCode();
	void SendCode();


	UFUNCTION()
	void OnButton0Pressed();
	UFUNCTION()
	void OnButton1Pressed();
	UFUNCTION()
	void OnButton2Pressed();
	UFUNCTION()
	void OnButton3Pressed();
	UFUNCTION()
	void OnButton4Pressed();
	UFUNCTION()
	void OnButton5Pressed();
	UFUNCTION()
	void OnButton6Pressed();
	UFUNCTION()
	void OnButton7Pressed();
	UFUNCTION()
	void OnButton8Pressed();
	UFUNCTION()
	void OnButton9Pressed();
	UFUNCTION()
	void OnButtonEnterPressed();
	UFUNCTION()
	void OnButtonCancelPressed();
};
