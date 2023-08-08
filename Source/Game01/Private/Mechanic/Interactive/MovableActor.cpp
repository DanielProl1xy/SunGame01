// Fill out your copyright notice in the Description page of Project Settings.


#include "Game01/Public/Mechanic/Interactive/MovableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Game01/Game01.h"
#include "DrawDebugHelpers.h"



AMovableActor::AMovableActor()
{
    PrimaryActorTick.bCanEverTick = true;
    MoveType = EMoveType::PUSH_PULL;
    RootPrimitive = Cast<UPrimitiveComponent>(RootComponent);
    if(!RootPrimitive)
    {
        UE_LOG(LogMovableActor, Error, TEXT("%s : Failed to cast RootComponent to RootPrimitive"), *GetName());
    }
}

void AMovableActor::BeginPlay()
{    
    Super::BeginPlay();

    LoadConfig();
    
    
    SetActorTickEnabled(false);
}

void AMovableActor::StartInteract_Implementation()
{
    AActor* temp = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());
    PlayerPawn = Cast<APlayerCharacter>(temp);
    if(PlayerPawn)
    {
        UE_LOG(LogMovableActor, Log, TEXT("Interaction wtih %s started"), *GetName());        
        PlayerPawn->bIsInteracting = true;
        PlayerPawn->CurrentInteraction = this;

        if(MoveType == EMoveType::AIM)
            RootPrimitive->SetEnableGravity(false);

        SetActorTickEnabled(true);
        return;
    }
    UE_LOG(LogMovableActor, Error, TEXT("Interaction wtih %s can not be started"), *GetName());
}

void AMovableActor::StopInteract_Implementation()
{
    SetActorTickEnabled(false);
    RootPrimitive->SetEnableGravity(true);
    PlayerPawn->bIsInteracting = false;
    UE_LOG(LogMovableActor, Warning, TEXT("Interaction wtih %s ended"), *GetName());
}

void AMovableActor::Tick(float delta)
{
    Super::Tick(delta);

    if(PlayerPawn == nullptr)
        return;

    FVector between = GetActorLocation() - PlayerPawn->GetActorLocation();
    if(between.Size() > MaximumInteractionDistace)
    {
        
        UE_LOG(LogMovableActor, Warning, TEXT("Interaction wtih %s corrupted because of high distance!"), *GetName());
        IInteractable::Execute_StopInteract(this);
        return;
    }
    if(between.Size() < MinimumMoveDistance)
    {
        return;
    }

    MoveActor();

}

void AMovableActor::MoveActor()
{
    if(PlayerPawn == nullptr)
        return;
    if(RootPrimitive == nullptr)
        return;
   
    switch (MoveType)
    {
    case EMoveType::PUSH_PULL:
        PushPullActor();
        break;
    case EMoveType::AIM:
        MoveActorByAIM();
        break;
    case EMoveType::ALONG_SURFACE:
        MoveActorAlongSurface();
        break;
        
    }
}

void AMovableActor::PushPullActor()
{
    const float inputForward = PlayerPawn->GetInputAxisValue(FName("Forward"));
    
    if(inputForward == 0)
        return;
    if(!RootPrimitive)
    {
        UE_LOG(LogMovableActor, Error, TEXT("%s : RootPrimitive is wrong target"), *GetName());
        return;
    }

    const FVector between = PlayerPawn->GetActorLocation() - GetActorLocation();
    const FVector requiredPos = PlayerPawn->GetActorLocation() - between.GetSafeNormal2D() * 100;

    const FVector moveDirection = GetActorLocation() - requiredPos;
    RootPrimitive->AddImpulse(moveDirection.GetSafeNormal2D() * -PushPullStrenght);

    DrawDebugSphere(GetWorld(), requiredPos, 30, 16, FColor::Blue, false, 0.01f, 0.0f, 1.f);
}

void AMovableActor::MoveActorByAIM()
{   
    const FVector moveTo = PlayerPawn->GetHandlePosition();
    const FVector direction = GetActorLocation() - moveTo;

    if(direction.Size() == 0)
        return;

    AddActorWorldOffset(direction * -0.5, true);

    const FVector cameraForward = PlayerPawn->GetPawnViewLocation() - moveTo;
    const FRotator modifiedRotation = cameraForward.Rotation();
    SetActorRotation(modifiedRotation);

    DrawDebugLine(GetWorld(), GetActorLocation(), moveTo, FColor::Red, false, 0.01f, 0, 1.f);
    DrawDebugSphere(GetWorld(), moveTo, 30, 16, FColor::Green, false, 0.01f, 0.0f, 1.f);
}

void AMovableActor::MoveActorAlongSurface()
{
    
}