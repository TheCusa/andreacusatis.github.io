// Fill out your copyright notice in the Description page of Project Settings.

#include "Keypad.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Components/WidgetComponent.h"
#include "CoopGame/Core/PlayerControllers/AgentPlayerController.h"
#include "CoopGame/Characters/Agent/AgentCharacter.h"
#include "CoopGame/Widgets/KeypadWidget.h"
#include "CoopGame/Core/Puzzle/Utils.h"



// Sets default values
AKeypad::AKeypad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the box collision component
	KeypadInteractionCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderComponent"));
	KeypadInteractionCollider->InitBoxExtent(FVector(30.0f, 30.0f, 30.0f));

	// Set collision as query only for overlaps events only with pawn
	KeypadInteractionCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KeypadInteractionCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	KeypadInteractionCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	KeypadInteractionCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	// Set box collision component as root component
	RootComponent = KeypadInteractionCollider;

	// Create the mesh component and attach it to root component
	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	KeypadMesh->SetupAttachment(RootComponent);

	// Create and setup Widget Component
	KeypadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("KeypadWidgetComponent"));
	KeypadWidgetComponent->SetupAttachment(RootComponent);

	// Configure widget settings
	KeypadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	KeypadWidgetComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void AKeypad::BeginPlay()
{
	Super::BeginPlay();

	// Delegate the overlap functions
	KeypadInteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnInteractionBoxOverlapBegin);
	KeypadInteractionCollider->OnComponentEndOverlap.AddDynamic(this, &AKeypad::OnInteractionBoxOverlapEnd);

	// "Subscribe" to a delegate event on KeypadWideg 
	// when the event occour, execute HandleWidgetCodeSent function
	if (KeypadWidgetComponent)
	{
		if (UUserWidget* UIWidget = KeypadWidgetComponent->GetUserWidgetObject())
		{
			if (UKeypadWidget* KeypadUI = Cast<UKeypadWidget>(UIWidget))
			{
				KeypadUI->OnCodeEntered.AddDynamic(this, &AKeypad::HandleWidgetCodeSent);
			}
		}
	}

	GenerateNewCode();
	
}

// Called every frame
void AKeypad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Do something when player interact with this object
void AKeypad::ExecuteAction()
{
	if (!(KeypadWidgetComponent->IsVisible()))
	{
		AgentPC->ServerRequestSaveCodeIntoGameState(this);
		ShowUIChangeInputMode();
	}
	else
	{
		HideUIChangeInputMode();
	}
}

void AKeypad::HandleWidgetCodeSent(const TArray<int8>& SentCode)
{
	AgentPC->SendArrayCode(SentCode, GameUserWidget::DigitDisplay);
	AgentPC->ServerRequestCheckCode(this, SentCode);
}

void AKeypad::Server_CheckCode_Implementation(const TArray<int8>& Array)
{
	if (Utils::CheckEquals(Array, Code))
	{
		OpenDoor();
		GenerateNewCode();
		if (IsValid(TargetDoor) && TargetDoor->DoorType == EDoorType::Gate)
		{
			Cast<ACoopGameState>(GetWorld()->GetGameState())->SetAlarm(false);
		}
	}
}

void AKeypad::Server_SaveCodeIntoGameState_Implementation()
{
	if (HasAuthority())		// only on server
	{
		ACoopGameState* GameStateRef = Cast<ACoopGameState>(GetWorld()->GetGameState());
		if (GameStateRef)
		{
			GameStateRef->CodePuzzleSolution = Code;
			Utils::DebugShowCode(GameStateRef->CodePuzzleSolution, "CodePuzzleSolution: ");
		}
	}
}

void AKeypad::ShowUIChangeInputMode()
{
	KeypadWidgetComponent->SetVisibility(true);

	FInputModeGameAndUI InputMode;

	if (AgentPC)
	{
		AAgentCharacter* AgentRef = Cast<AAgentCharacter>(AgentPC->GetPawn());
		AgentRef->LockCharacterMovement();
		AgentPC->SetInputMode(InputMode);
		AgentPC->SetShowMouseCursor(true);
	}
}

void AKeypad::HideUIChangeInputMode()
{
	KeypadWidgetComponent->SetVisibility(false);

	FInputModeGameOnly InputMode;

	if (AgentPC)
	{
		AAgentCharacter* AgentRef = Cast<AAgentCharacter>(AgentPC->GetPawn());
		AgentRef->UnlockCharacterMovement();
		AgentPC->SetInputMode(InputMode);
		AgentPC->SetShowMouseCursor(false);
	}
}

void AKeypad::GenerateNewCode()
{
	if (HasAuthority())
	{
		Code.Empty();
		Code = Utils::GenerateCode(4);
		UE_LOG(LogTemp, Warning, TEXT("CODE GENERATED"));
	}
}

void AKeypad::OpenDoor()
{
	if (!HasAuthority())
	{
		return;
	}

	if (TargetDoor)
	{
		if (!TargetDoor->IsOpen())
		{
			TargetDoor->OpenDoor();
		}
		else
		{
			TargetDoor->CloseDoor();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetDoor not assigned!"));
	}
}


// ------ OVERLAPS FUNCTIONS -------
void AKeypad::OnInteractionBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || (OtherActor == this))
	{
		return;
	}

	AAgentCharacter* OverlapCharacter = Cast<AAgentCharacter>(OtherActor);
	if (OverlapCharacter)
	{
		// Check if OverlapChracter is the local player. It's needed to execute this event only in local
		if (!OverlapCharacter->IsLocallyControlled())
		{
			return;
		}
		OverlapCharacter->SetNearbyInteractableObject(this);
		AgentPC = Cast<AAgentPlayerController>(OverlapCharacter->GetController());
	}
}

void AKeypad::OnInteractionBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor || (OtherActor == this))
	{
		return;
	}

	AAgentCharacter* OverlapCharacter = Cast<AAgentCharacter>(OtherActor);
	if (OverlapCharacter)
	{
		// Check if OverlapChracter is the local player. It's needed to execute this event only in local
		if (!OverlapCharacter->IsLocallyControlled())
		{
			return;
		}

		//UE_LOG(LogTemp, Warning, TEXT("END OVERLAP"));
		OverlapCharacter->ClearNearbyInteractableObject(this);
		AgentPC = nullptr;
	}
}