#include "Board.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "MarbleGameMode.h"
#include "Kismet/KismetSystemLibrary.h"


ABoard::ABoard()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
    RootComponent = MeshComp;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(RootComponent);

}

void ABoard::BeginPlay()
{

    BallRef = Cast<ABall>(UGameplayStatics::GetActorOfClass(this, ABall::StaticClass()));
    if (BoxCollision)
    {
        BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoard::OnTriggerEnter);
        BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABoard::OnTriggerExit);
        BoxCollisionLocation = BoxCollision->GetComponentLocation();

    }
    GameModeRef = Cast<AMarbleGameMode>(UGameplayStatics::GetGameMode(this));
}

void ABoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("LateralTilt", this, &ABoard::TiltLateral);  
    PlayerInputComponent->BindAxis("ForwardTilt", this, &ABoard::TiltForward); 
    PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &ABoard::QuitGame);

}

void ABoard::TiltForward(float Value)
{
    if (MeshComp && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        FRotator CurrentRotation = MeshComp->GetComponentRotation();
        CurrentRotation.Pitch += Value * MouseSensitivity * DeltaTime;
        CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch, -MaxAngle, MaxAngle);
        MeshComp->SetWorldRotation(CurrentRotation);
    }
}

void ABoard::TiltLateral(float Value)
{
    if (MeshComp && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        FRotator CurrentRotation = MeshComp->GetComponentRotation();
        CurrentRotation.Roll += Value * MouseSensitivity * DeltaTime;
        CurrentRotation.Roll = FMath::Clamp(CurrentRotation.Roll, -5.f, 5.f);
        MeshComp->SetWorldRotation(CurrentRotation);
    }
}
void ABoard::OnTriggerEnter(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (GameModeRef && !GameModeRef->b_HasAlreadyWon)
    {
        GameModeRef->b_HasAlreadyWon = true;
        GameModeRef->HandleVictory();
    }
    
    
}
void ABoard::OnTriggerExit(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    GameModeRef->b_HasAlreadyWon = false;

}
void ABoard::QuitGame()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}