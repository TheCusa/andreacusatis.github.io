// Fill out your copyright notice in the Description page of Project Settings.


#include "Alarm.h"

#include "CoopGame/Core/CoopGameState.h"

// Sets default values
AAlarm::AAlarm()
{
	PrimaryActorTick.bCanEverTick = false;
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotlightComponent"));
	RootComponent = SpotLight; 
}

void AAlarm::BeginPlay()
{
	Super::BeginPlay();
	if (ACoopGameState* GameStateRef = Cast<ACoopGameState>(GetWorld()->GetGameState()))
	{
		GameStateRef->OnAlarmChanged.AddDynamic(this, &AAlarm::HandleAlarmChanged);
	}
	
}

// Called every frame
void AAlarm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAlarm::HandleAlarmChanged(bool bNewState)
{
	if (HasAuthority())
	{
		GEngine-> AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Alarm state changed: %s"), bNewState ? TEXT("ON") : TEXT("OFF")));
		SpotLight->SetVisibility(bNewState);
	}
}

