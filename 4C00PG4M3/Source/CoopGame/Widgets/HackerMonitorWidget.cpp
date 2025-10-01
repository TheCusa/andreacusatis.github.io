// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame/Widgets/HackerMonitorWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "DigitDisplayWidget.h"
#include "HackerLaserPuzzleWidget.h"
#include "HackerPianoWidget.h"

void UHackerMonitorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CreateAndAddWidgets();

	//Binding all the buttons
	if (KeypadButton)
	{
		KeypadButton->OnPressed.AddDynamic(this, &UHackerMonitorWidget::OnKeypadButtonPressed);
	}
	if (CameraButton)
	{
		CameraButton->OnPressed.AddDynamic(this, &UHackerMonitorWidget::OnCameraButtonPressed);
	}

	if (PianoButton)
	{
		PianoButton->OnPressed.AddDynamic(this, &UHackerMonitorWidget::OnPianoButtonPressed);
	}
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHackerMonitorWidget::OnKeypadButtonPressed()
{
	if (UDigitDisplayWidget* DigitWidget = Cast<UDigitDisplayWidget>(WidgetSwitcher->GetWidgetAtIndex(0)))
	{
		SwitchToWidget(0);
	}

}

void UHackerMonitorWidget::OnCameraButtonPressed()
{
	SwitchToWidget(1);
}

void UHackerMonitorWidget::OnPianoButtonPressed()
{
	SwitchToWidget(2);
}

void UHackerMonitorWidget::SwitchToWidget(int32 WidgetIndex)
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		WidgetSwitcher->SetActiveWidgetIndex(WidgetIndex);
	}
}

void UHackerMonitorWidget::CreateAndAddWidgets()
{
    if (!WidgetSwitcher) return;

    if (DigitDisplayWidget)
    {
        DigitDisplayWidgetInstance = CreateWidget<UDigitDisplayWidget>(this, DigitDisplayWidget);

        if (DigitDisplayWidgetInstance)
        {
            WidgetSwitcher->AddChild(DigitDisplayWidgetInstance);
        }
    }

    if (HackerLaserPuzzleWidget)
    {
        HackerLaserPuzzleWidgetInstance = CreateWidget<UHackerLaserPuzzleWidget>(this, HackerLaserPuzzleWidget);
        if (HackerLaserPuzzleWidgetInstance)
        {
            WidgetSwitcher->AddChild(HackerLaserPuzzleWidgetInstance);
        }
    }

	
	if (HackerPianoWidget)
    {
        HackerPianoWidgetInstance = CreateWidget<UHackerPianoWidget>(this, HackerPianoWidget);
        if (HackerPianoWidgetInstance)
        {
            WidgetSwitcher->AddChild(HackerPianoWidgetInstance);
        }
    }
}

UWidgetSwitcher* UHackerMonitorWidget::GetWidgetSwitcher()
{
	return WidgetSwitcher;
}
