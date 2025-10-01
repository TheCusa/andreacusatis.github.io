// Fill out your copyright notice in the Description page of Project Settings.


#include "KeypadWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UKeypadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Binding all the buttons

	if (KeyButton0)
	{
		KeyButton0->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton0Pressed);		
	}
	if (KeyButton1)
	{
		KeyButton1->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton1Pressed);

	}
	if (KeyButton2)
	{
		KeyButton2->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton2Pressed);

	}
	if (KeyButton3)
	{
		KeyButton3->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton3Pressed);

	}
	if (KeyButton4)
	{
		KeyButton4->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton4Pressed);

	}
	if (KeyButton5)
	{
		KeyButton5->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton5Pressed);

	}
	if (KeyButton6)
	{
		KeyButton6->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton6Pressed);

	}
	if (KeyButton7)
	{
		KeyButton7->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton7Pressed);

	}
	if (KeyButton8)
	{
		KeyButton8->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton8Pressed);

	}
	if (KeyButton9)
	{
		KeyButton9->OnPressed.AddDynamic(this, &UKeypadWidget::OnButton9Pressed);

	}
	if (KeyButtonEnter)
	{
		KeyButtonEnter->OnPressed.AddDynamic(this, &UKeypadWidget::OnButtonEnterPressed);

	}
	if (KeyButtonCancel)
	{
		KeyButtonCancel->OnPressed.AddDynamic(this, &UKeypadWidget::OnButtonCancelPressed);

	}
	MaxCodeNumbers = 4;
	
}

void UKeypadWidget::DisplayOnScreen(const int32 number)
{
	
	if (FirstSlotText && FirstSlotText->GetText().IsEmpty())
	{
		FirstSlotText->SetText(FText::AsNumber(number));
	}
	else if (SecondSlotText && SecondSlotText->GetText().IsEmpty())
	{
		SecondSlotText->SetText(FText::AsNumber(number));
	}
	else if (ThirdSlotText && ThirdSlotText->GetText().IsEmpty())
	{
		ThirdSlotText->SetText(FText::AsNumber(number));
	}
	else if (FourthSlotText && FourthSlotText->GetText().IsEmpty())
	{
		FourthSlotText->SetText(FText::AsNumber(number));
	}

}
//adding input numbers to a TArray
void UKeypadWidget::AddCodeNumber(const int32 number)
{
	if (InputCode.Num() < MaxCodeNumbers)
	{
		InputCode.Add(number);
		DisplayOnScreen(number);
	}
}
//Centralizing buttons functions
void UKeypadWidget::ButtonClickedHandler(const int32 buttonID)
{
	if (buttonID < 10)
	{
		AddCodeNumber(buttonID);

	}
	else if (buttonID == 10)
	{
		SendCode();
		ClearCode();
	}
	else if (buttonID == 11)
	{
		ClearCode();
	}

}
void UKeypadWidget::SendCode()
{
	if (InputCode.Num() == MaxCodeNumbers) //checking if all digits are inserted
	{
		OnCodeEntered.Broadcast(InputCode);		// Send a delegate event

		//Debug purpose
		for (int i = 0; i < InputCode.Num(); ++i)
		{
			UE_LOG(LogTemp, Warning, TEXT("cifra: %d"), InputCode[i]);
		}
	}
	
}

void UKeypadWidget::ClearCode()
{
	InputCode.Empty();
	if (FirstSlotText)
	{
		FirstSlotText->SetText(FText::GetEmpty());
		SecondSlotText->SetText(FText::GetEmpty());
		ThirdSlotText->SetText(FText::GetEmpty());
		FourthSlotText->SetText(FText::GetEmpty());
	}
}
void UKeypadWidget::OnButton0Pressed()
{
	ButtonClickedHandler(0);
}
void UKeypadWidget::OnButton1Pressed()
{
	ButtonClickedHandler(1);

}
void UKeypadWidget::OnButton2Pressed()
{
	ButtonClickedHandler(2);

}
void UKeypadWidget::OnButton3Pressed()
{
	ButtonClickedHandler(3);

}
void UKeypadWidget::OnButton4Pressed()
{
	ButtonClickedHandler(4);

}
void UKeypadWidget::OnButton5Pressed()
{
	ButtonClickedHandler(5);

}
void UKeypadWidget::OnButton6Pressed()
{
	ButtonClickedHandler(6);

}
void UKeypadWidget::OnButton7Pressed()
{
	ButtonClickedHandler(7);

}
void UKeypadWidget::OnButton8Pressed()
{
	ButtonClickedHandler(8);

}
void UKeypadWidget::OnButton9Pressed()
{
	ButtonClickedHandler(9);

}
void UKeypadWidget::OnButtonEnterPressed()
{
	ButtonClickedHandler(10);

}
void UKeypadWidget::OnButtonCancelPressed()
{
	ButtonClickedHandler(11);

}
 
