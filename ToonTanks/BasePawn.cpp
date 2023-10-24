// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Tank.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Creates capsule component
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	//Visual effects
		//Spawning visual effects
		if (DeathParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle,GetActorLocation(),GetActorRotation());
		}
}


void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// Calculates target vector from turret mesh location to target given as function input (mouse for tank, enemy for turret)
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	// Obtains rotation angles from target vector, while zeroing pitch and roll. Only yaw (Z axis rotation) is needed
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	// Done before FRotator LookAtRotation = ToTarget.Rotation();
	// Done before LookAtRotation.Pitch = 0.f;
	// Done before LookAtRotation.Yaw = 0.f;

	// Adds rotation to turret mesh, commented because it is too direct, needs an interpolation
	//TurretMesh->SetWorldRotation(LookAtRotation);

	//Adds rotation to turret mesh through an interpolation
	TurretMesh->SetWorldRotation(FMath::RInterpTo(	TurretMesh->GetComponentRotation(),
													LookAtRotation,UGameplayStatics::GetWorldDeltaSeconds(this),
													15.f));
}

void ABasePawn::Fire()
{
	FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileSPawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();
	DrawDebugSphere(GetWorld(),
                	ProjectileSpawnPointLocation,
            		25.f,
                    12,
                    FColor::Blue,
                    false,
                    3.f);
	
	//Spawns projectile and stores it in a variable. 'Auto' automatically takes the class type and assigns it to the variable
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
										ProjectileSpawnPointLocation,
										ProjectileSPawnPointRotation);
	Projectile->SetOwner(this);

	//Testing
	auto MyOwner = Projectile->GetOwner();
		if(MyOwner->GetClass()->IsChildOf(ATank::StaticClass()))
	{
			UE_LOG(LogTemp, Warning, TEXT("The Tank has fired: %s"),*MyOwner->GetName());
			Tank->CountAmmo();
	}
}
