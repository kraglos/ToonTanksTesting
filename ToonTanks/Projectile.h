// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float InitProjectileSpeed = 10.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxProjectileSpeed = 50.f;

	//Visual effect
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystemComponent* ProjectileTrail;

	//Testing
	class ATank* Tank;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Damage to apply when hit
	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
