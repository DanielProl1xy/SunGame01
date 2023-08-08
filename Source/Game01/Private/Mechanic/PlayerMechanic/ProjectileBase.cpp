// Fill out your copyright notice in the Description page of Project Settings.

#include "Game01/Public/Mechanic/PlayerMechanic/ProjectileBase.h"
#include "Game01/Game01.h"


AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));	
	CollisionComponent->SetVisibility(true);
	CollisionComponent->bHiddenInGame = false;
	RootComponent = CollisionComponent;

	LoadConfig();
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	CollisionComponent->SetSphereRadius(CollisionRadius);
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->MaxSpeed = Speed;
	
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->SetActive(true);

}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();		
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

;

	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	UE_LOG(LogProjectile, Display, TEXT("Projectile spawned"));
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor)
	{
		UE_LOG(LogProjectile, Display, TEXT("Projectile hit %s" ), *OtherActor->GetName());
	}	
	this->Destroy();
}


