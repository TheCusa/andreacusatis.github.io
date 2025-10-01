// Fill out your copyright notice in the Description page of Project Settings.


#include "HackerLaserPuzzleWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CoopGame/Core/Puzzle/SecurityCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"

void UHackerLaserPuzzleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PrevCameraButton->OnPressed.AddDynamic(this, &UHackerLaserPuzzleWidget::OnPrevCameraPressed);
	NextCameraButton->OnPressed.AddDynamic(this, &UHackerLaserPuzzleWidget::OnNextCameraPressed);
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASecurityCamera::StaticClass(), FoundCameras);
	UE_LOG(LogTemp, Display, TEXT("Cameras: %d"), FoundCameras.Num());

	for (AActor* Actor : FoundCameras)
	{
		ASecurityCamera* SecurityCamera = Cast<ASecurityCamera>(Actor);
		if (SecurityCamera)
		{
			SecurityCameras.Add(SecurityCamera);
		}
	}	

	SecurityCameras.Sort([](const ASecurityCamera& A, const  ASecurityCamera& B)
	{
		return A.SortingOrder < B.SortingOrder;
	});
	
	CameraFeedIndex = 0;

	if (CameraFeedImage)
	{
		CameraFeedMaterial = CameraFeedImage->GetDynamicMaterial();
	}
	
	SetFeed();
}

void UHackerLaserPuzzleWidget::NativeDestruct()
{
	Super::NativeDestruct();
	for (ASecurityCamera* Camera : SecurityCameras)
	{
		Camera->SetActive(false);
	}
	UE_LOG(LogTemp, Warning, TEXT("HackerLaserPuzzleWidget DESTRUCTED!"));
	
}

void UHackerLaserPuzzleWidget::SetFeed()
{
	if (SecurityCameras.Num() == 0 || !CameraFeedMaterial) return;
	
	if (CameraFeedIndex < 0)
	{
		CameraFeedIndex = SecurityCameras.Num() - 1;
	}
	else if (CameraFeedIndex >= SecurityCameras.Num())
	{
		CameraFeedIndex = 0;
	}

	ShownCameraText->SetText(FText::FromString("Displaying Camera: " + FString::FromInt(CameraFeedIndex + 1)));

	for (ASecurityCamera* Camera : SecurityCameras)
	{
		Camera->SetActive(false);
	}
	SecurityCameras[CameraFeedIndex]->SetActive(true);
	if (SecurityCameras[CameraFeedIndex]->CameraCaptureComponent && SecurityCameras[CameraFeedIndex]->CameraCaptureComponent->TextureTarget)
	{
		CameraFeedMaterial->SetTextureParameterValue(TEXT("CameraTexture"), SecurityCameras[CameraFeedIndex]->CameraCaptureComponent->TextureTarget);
	}

	UE_LOG(LogTemp, Display, TEXT("SetFeed to Camera Index: %i, using Dynamic Material"), CameraFeedIndex);
}

void UHackerLaserPuzzleWidget::OnLeftKeyPressed()
{
	CameraFeedIndex--;
	SetFeed();
}

void UHackerLaserPuzzleWidget::OnRightKeyPressed()
{
	CameraFeedIndex++;
	SetFeed();
}

void UHackerLaserPuzzleWidget::OnNextCameraPressed()
{
	CameraFeedIndex++;
	SetFeed();
}

void UHackerLaserPuzzleWidget::OnPrevCameraPressed()
{
	CameraFeedIndex--;
	SetFeed();
}



