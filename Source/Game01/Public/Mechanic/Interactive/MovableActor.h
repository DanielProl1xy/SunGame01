// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Game01/Public/PlayerCharacter.h"
#include "MovableActor.generated.h"

UENUM(BlueprintType)
enum class EMoveType : uint8
{   
    PUSH_PULL       UMETA(DisplayName="Move actor by push/pull method"),
    AIM             UMETA(DisplayName="Move actor by AIM"),
    ALONG_SURFACE   UMETA(DisplayName="Move actor along surface by AIM")
};

UCLASS(Config=Game)
class GAME01_API AMovableActor : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    APlayerCharacter* PlayerPawn;

public:
    UPROPERTY(Config, BlueprintReadOnly)
    float MinimumMoveDistance;
    UPROPERTY(Config, BlueprintReadOnly)
	float MaximumInteractionDistace;
    UPROPERTY(Config, BlueprintReadOnly)
    float SafeDistance;
    UPROPERTY(Config, BlueprintReadOnly)
    float PushPullStrenght;
    UPROPERTY(EditAnywhere, Category="Movement")
    EMoveType MoveType;
    UPROPERTY(BlueprintReadWrite, Category="Components")
    UPrimitiveComponent* RootPrimitive;

public:
    AMovableActor();
    void Tick(float delta) override;
    void BeginPlay() override;

public:
    virtual void StartInteract_Implementation() override;
    virtual void StopInteract_Implementation() override;
    virtual void MoveActor();

private:
    void PushPullActor();
    void MoveActorByAIM();
    void MoveActorAlongSurface();
};