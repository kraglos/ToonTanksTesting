// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(InFireRange()) //Calls InFireRange to check if we are in fire range
    {
             RotateTurret(Tank->GetActorLocation()); //Rotates turret
    }  
}

void ATower::HandleDestruction()
{
    // Call the 'super' (the function of the parent class, BasePawn) to invoke visual and sound effects
    Super::HandleDestruction();
    Destroy();
}

void ATower::BeginPlay()
{
    Super::BeginPlay();
    // Casting to get pawn from player controller
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));
    //Get world timer manager
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);

}

void ATower::CheckFireCondition()
{
    if(InFireRange()) //Check if we are in fire range
    {
        // If in range, fire
        Fire();
    }
}

bool ATower::InFireRange()
{
    if (Tank)
    {
        // Find distance to tank
         float Distance = FVector::Dist(GetActorLocation(),Tank->GetActorLocation());
        // Check to see if the tank is in range
         if (Distance <= FireRange)
         {
            return true;
         }       
    }
    return false; // Only gets here if function did not enter through the IF statement
}