// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileBase.generated.h"

UCLASS(Config=Game)
class GAME01_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectileBase();

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(Config, EditAnywhere, Category = "Components")
	float CollisionRadius;

	UPROPERTY(Config, EditAnywhere,Category = "Movement")
	float Speed;

	UPROPERTY(Config, EditAnywhere, Category = "Damage")
	float Damage;

protected:

	void BeginPlay();

	void PostInitializeComponents() override;

public:

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
