// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
//Adding gameplaystatics to be able to get player pawn
#include "Kismet/GameplayStatics.h"
//Include tank header to be able to cast from APawn to ATank in BeginPlay
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "Particles/ParticleSystemComponent.h"


	void AToonTanksGameMode::ActorDied(AActor *DeadActor)
    {
        // Checks what to do when actor is tank
        if (DeadActor == Tank)
        {
            Tank->HandleDestruction();

            // Checks if player controller exists, then disables input and hides cursor
            if (ToonTanksPlayerController)
            {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
            }
            GameOver(false); // If actor is destroyed, game is lost
        }
        //Casts to a variable declared inside the else if statement
        //If the dead actor is a tower, then do this
        else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
        {
            DestroyedTower->HandleDestruction();
            TargetTowers--; // Decreases the amount of towers in the world
            if(TargetTowers == 0)
            {
                GameOver(true);
            }
        }
    }

    void AToonTanksGameMode::BeginPlay()
    {
        Super::BeginPlay();

        //Starts game delay counter at begin play, and handles other 'game start' functionalities
        HandleGameStart();

    }

    void AToonTanksGameMode::HandleGameStart()
    {
        TargetTowers = GetTargetTowerCount(); //Counts how many towers are there in our world
        // Gets player pawn in the form of a tank pointer. Uses 0 as argument because that's the player controller
        Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
        //set player controller pointer
        ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));

        StartGame(); // Blueprint implementable event, done in blueprints
        //Start countdown timer before we enable user input
        if (ToonTanksPlayerController)
        {
       //     //Disables input first
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        //    //Start timer
            FTimerHandle PlayerEnableTimerHandle;
        //    //Create local FTimerDelegate
            FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
                ToonTanksPlayerController,
                &AToonTanksPlayerController::SetPlayerEnabledState,
                true);
            GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
                                            PlayerEnableTimerDelegate,
                                            StartDelay,
                                            false);
        }
    }

    	int32 AToonTanksGameMode::GetTargetTowerCount()
        {
            TArray<AActor*> Towers;
            UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
            return Towers.Num();
        }