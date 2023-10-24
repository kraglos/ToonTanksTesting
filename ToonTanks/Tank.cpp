// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ToonTanksGameMode.h"


//Constructor
ATank::ATank()
{
    //Creates camera arm
    TankSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Tank Spring Arm"));
    TankSpringArm->SetupAttachment(RootComponent);

    //Creates camera
    TankCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Tank Camera"));
    TankCamera->SetupAttachment(TankSpringArm);
} 

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent); //Calls parent class

    // Get the player controller
    auto PlayerController = Cast<APlayerController>(GetController());
 
    // Get the local player enhanced input subsystem
    auto EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    //Add the input mapping context
    EISubsystem->AddMappingContext(InputMapping, 0);
 
    // Get the EnhancedInputComponent
    auto PlayerEIcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//Bind Move() to the mapping
	//BindAction for enhanced system takes Action, ETriggerEvent, object, and function
	//ETriggerEvent is an enum, where Triggered means "button is held down".
	PlayerEIcomponent->BindAction(InputMoveForward, ETriggerEvent::Triggered, this, &ATank::Move);
    PlayerEIcomponent->BindAction(InputTurn, ETriggerEvent::Triggered, this, &ATank::Turn); // Triggered = while pressed: keeps calling function
    PlayerEIcomponent->BindAction(InputFire, ETriggerEvent::Started, this, &ATank::Fire); // Started = when pressed once, call once (needs a release to call again)

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

    //We cast because PlayerControllerRef is of type APlayerController, but our reference controller is not, although the object type is 'the same'.
    //This is because the GetController return is a APlayerController type, which is stored as a controller.
    TankPlayerController = Cast<APlayerController>(GetController());

    //Testing
    CurrentAmmo = MaxAmmo;
    ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(TankPlayerController)
    {
        FHitResult HitResult; // Create HitResult to store collision result data

        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult); //Call GetHitResultUnderCursor and store in HitResult

        // Call debug sphere to see where the cursor points
        DrawDebugSphere(GetWorld(),
                        HitResult.ImpactPoint,
                        30.f,
                        12,
                        FColor::Red,
                        false,
                        -1.f);
    // Assigns turret rotation using function defined in BasePawn
    RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    // Call the 'super' (the function of the parent class, BasePawn) to invoke visual and sound effects
    Super::HandleDestruction(); 
    // Hides actor and makes it non-movable by disabling ticking rather than destroying it.
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

void ATank::Move(const FInputActionValue & Value)
{
	//To bind to axis mapping: SetupPlayerInputComponent
	UE_LOG(LogTemp, Display, TEXT("Float value: %f"), Value.Get<float>());


    //Get DeltaTime to fps-lock movement
    float SourcedTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    //Assign local X direction to movement value: X = Value * Speed * DeltaTime
    FVector DeltaLocation(0.f); // Initialises components at 0. Another method: DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value.Get<float>() * TankSpeed * SourcedTime;
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(const FInputActionValue & Value)
{
    //Attach rotation
    FRotator DeltaRotation = FRotator::ZeroRotator;
    //Yaw = Value * DeltaTime * TurnRate
    DeltaRotation.Yaw = Value.Get<float>() * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this); //Done as for Move, but without intermediate time variable
    AddActorLocalRotation(DeltaRotation, true);
}

//Testing
void ATank::CountAmmo()
{
    if (CurrentAmmo <= 1)
    {
        ToonTanksGameMode->GameOver(false);
        UE_LOG(LogTemp, Warning, TEXT("The tank has run out of ammo!"));
    }
    else
    {
        CurrentAmmo--;
    }
}
