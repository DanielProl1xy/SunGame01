// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game01/Public/Mechanic/Interactive/Interactable.h"
#include "Game01/Public/Settings/PlayerAge.h"
#include "PlayerCharacter.generated.h"



UCLASS(Config=Game)
class GAME01_API APlayerCharacter : public ACharacter
{

	GENERATED_BODY()

private:

	FPlayerAge Age;

public:

	APlayerCharacter();

public:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCharacterMovementComponent* MovementComp;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UTimelineComponent* DashTimeLine;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UCurveFloat* DashCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DashMaxSpeed;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HandleDistance;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly)
	float ItemMinDistance;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly)
	float ItemMaxDistance;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly)
	float InteractMaxDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* CurrentInteraction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsInteracting;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	bool bIsShooting;
	bool bCanShoot;
	bool bIsDashing;
	bool bIsJumping;	
	FVector dashDirection;

private:
	void StartShoot();
	void StopShoot();
	void LaunchProjectile() const;
	void SecondAttack();
	void Interact();
	
private:
	bool CanDash() const;
	void StartDash();
	void UpdateDashSpeed(const float speed);
	void StopDash();
	void MoveForward(const float value);
	void MoveRight(const float value);
	void Jump() override;
	void Landed(const FHitResult & Hit) override;
	void StartCrouch() { this->Crouch(); }
	void StopCrouch() { this->UnCrouch(); }
	void Scroll(const float value);

public:
	UFUNCTION(BlueprintCallable)
	void SetAge(const FPlayerAge newAge);

	UFUNCTION(BlueprintCallable)
	FVector GetHandlePosition() const;

};
