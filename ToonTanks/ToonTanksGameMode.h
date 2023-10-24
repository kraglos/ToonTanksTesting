// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

	// Implementing blueprint implementable event for screen text, can't be private. We can call it from C++, but we don't need to create a body for it here since it's done in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

protected:
	virtual void BeginPlay() override;


	
private:
	class ATank* Tank;
	class AToonTanksPlayerController* ToonTanksPlayerController;

	float StartDelay = 3.f; // Time delay between game load and game start

	void HandleGameStart(); // Function to handle when to allow the game to start

	int32 TargetTowers = 0; // Towers in our world
	
	int32 GetTargetTowerCount(); //Function that returns the amount of towers in our world


};
