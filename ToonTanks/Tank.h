// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"


/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	// Creates a PlayerController pointer 'getter', since the variable is declared here privately, and we need to access it in GameMode
	APlayerController* GetTankPlayerController() const{return TankPlayerController;}

	UFUNCTION()
	void CountAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

UPROPERTY(VisibleAnywhere, Category = "Components")
class USpringArmComponent* TankSpringArm;

UPROPERTY(VisibleAnywhere, Category = "Components")
class UCameraComponent* TankCamera;

// Input mapping. Mapping context and Input Action variables, to be set in blueprints
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
class UInputMappingContext* InputMapping;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
class UInputAction * InputMoveForward;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
class UInputAction * InputTurn;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
class UInputAction * InputRotateTurret;

UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
class UInputAction * InputFire;

// Speed for tank movement
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Movement", meta = (AllowPrivateAccess = "true"))
float TankSpeed = 100.0;
// Rotation for tank movement
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank Movement", meta = (AllowPrivateAccess = "true"))
float TurnRate= 100.0;

// Functions to allow controls
void Move(const FInputActionValue & Value);
void Turn(const FInputActionValue & Value);

//PlayerController
APlayerController* TankPlayerController;

	//Testing
	UPROPERTY(EditAnywhere)
	int32 MaxAmmo = 20;
	int32 CurrentAmmo = 0;
	class AToonTanksGameMode* ToonTanksGameMode;
	
};
