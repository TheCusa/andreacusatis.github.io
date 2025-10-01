// Fill out your copyright notice in the Description page of Project Settings.


#include "SecurityCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
ASecurityCamera::ASecurityCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create and setup Static Mesh Component
	CameraStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = CameraStaticMeshComponent;

	CameraCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraCaptureComponent"));
	CameraCaptureComponent->SetupAttachment(RootComponent);

	CameraCaptureComponent->bCaptureEveryFrame = false;
	CameraCaptureComponent->bCaptureOnMovement = false;
}

// Called when the game starts or when spawned
void ASecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	if (CameraCaptureComponent && !CameraCaptureComponent->TextureTarget)
	{
		UTextureRenderTarget2D* NewRenderTarget = NewObject<UTextureRenderTarget2D>(this);
		if (NewRenderTarget)
		{
			NewRenderTarget->InitAutoFormat(1280, 720);
			NewRenderTarget->UpdateResource();
			CameraCaptureComponent->TextureTarget = NewRenderTarget;
			UE_LOG(LogTemp, Warning, TEXT("Created dynamic Render Target for %s"), *GetName());
		}
	}
}

void ASecurityCamera::SetActive(bool bIsActive)
{
	if (CameraCaptureComponent)
	{
		CameraCaptureComponent->bCaptureEveryFrame = bIsActive;
	}
}


