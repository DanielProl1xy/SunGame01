// Fill out your copyright notice in the Description page of Project Settings.

#include "Game01/Public/PlayerCharacter.h"
#include "CoreMinimal.h"
#include "Game01/Public/Mechanic/PlayerMechanic/ProjectileBase.h"
#include "DrawDebugHelpers.h"
#include "Game01/Game01.h"
#include "GameFramework/CharacterMovementComponent.h"


APlayerCharacter::APlayerCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.55f;
	
	MovementComp = GetCharacterMovement();
	CapsuleComp = GetCapsuleComponent();

	MovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;

	Age.AgeId = 0;
	Age.bCanDash = false;
	Age.bCanShoot = true;
	Age.CapsuleHeight = 120;
	Age.CapsuleRadius = 25;

	SetAge(Age);	

	MovementComp->MaxWalkSpeed = 500;
	MovementComp->MaxWalkSpeedCrouched = 300;
	
/*
	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

	DashTimeLine = NewObject<UTimelineComponent>(this, FName("DashTimeLine"));
	this->BlueprintCreatedComponents.Add(DashTimeLine);
	
	DashTimeLine->SetLooping(false);

	onTimelineCallback.BindUFunction(this, FName("UpdateDashSpeed"));
	onTimelineFinishedCallback.BindUFunction(this, FName("StopDash"));

	DashTimeLine->AddInterpFloat(DashCurve, onTimelineCallback);
	DashTimeLine->SetTimelineFinishedFunc(onTimelineFinishedCallback);

	DashTimeLine->RegisterComponent();
*/
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadConfig();
	
	HandleDistance = ItemMinDistance;

	SetActorTickEnabled(false);	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsShooting)
	{
		LaunchProjectile();
	}
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed , this, &APlayerCharacter::StartShoot);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Released , this, &APlayerCharacter::StopShoot);

	PlayerInputComponent->BindAction(FName("Dash"), IE_Pressed, this, &APlayerCharacter::StartDash);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Scroll"), this, &APlayerCharacter::Scroll);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &APlayerCharacter::Jump);

	PlayerInputComponent->BindAction(FName("Crouch"), IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Released, this, &APlayerCharacter::StopCrouch);

	PlayerInputComponent->BindAction(FName("Interact"), IE_Pressed, this, &APlayerCharacter::Interact);
	

}

void APlayerCharacter::StartShoot()
{
	if(Age.bCanShoot == false)
		return;
	SetActorTickEnabled(true);
	bIsShooting = true;
}

void APlayerCharacter::StopShoot()
{
	SetActorTickEnabled(false);
	bIsShooting = false;
}

void APlayerCharacter::LaunchProjectile() const
{
	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50) + GetController()->GetControlRotation().Vector() * 100;
    FRotator SpawnRotation = GetController()->GetControlRotation();
    FActorSpawnParameters SpawnParams;
    GetWorld()->SpawnActor<AProjectileBase>(SpawnLocation, SpawnRotation, SpawnParams);
}

void APlayerCharacter::SecondAttack()
{
	UE_LOG(LogPlayerCharacter, Warning, TEXT("DO second attack"));
}

void APlayerCharacter::Scroll(const float value)
{
	if(value == 0)
		return;

	HandleDistance = FMath::Clamp(HandleDistance + value * 3, ItemMinDistance, ItemMaxDistance);
}

bool APlayerCharacter::CanDash() const
{
	return !(bIsJumping || bIsDashing) && Age.bCanDash;
}

void APlayerCharacter::StartDash() 
{
	UE_LOG(LogPlayerCharacter, Warning, TEXT("Do dash"));
}


void APlayerCharacter::UpdateDashSpeed(const float rate)
{
	float speed = rate * DashMaxSpeed;

	FVector impulse = dashDirection * speed;
	MovementComp->AddImpulse(impulse, true);
}

void APlayerCharacter::StopDash() 
{
	bIsDashing = false;
}

void APlayerCharacter::Interact()
{
	if(bIsInteracting)
	{
		if(CurrentInteraction)
			IInteractable::Execute_StopInteract(CurrentInteraction);
		bIsInteracting = false;
		CurrentInteraction = nullptr;
		UE_LOG(LogActor, Log, TEXT("Interaction ended"));
		return;
	}

	FVector startTrace = GetActorLocation();
	startTrace.Z += 20;
	FVector endTrace = GetControlRotation().Vector() * InteractMaxDistance + startTrace;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
    QueryParams.AddIgnoredActor(this);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, startTrace, endTrace, ECC_Visibility, QueryParams);

	DrawDebugLine(GetWorld(), startTrace, endTrace, FColor::Red, false, 5.f, 0, 1.f);

	if(Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		if(Hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_StartInteract(Hit.GetActor());
			UE_LOG(LogPlayerCharacter, Log, TEXT("Interaction started"));
		}
		else		
			UE_LOG(LogPlayerCharacter, Warning, TEXT("%s is not interactable"), *Hit.GetActor()->GetName());	
	}
	else
		UE_LOG(LogPlayerCharacter, Error, TEXT("Interaction is not succesful"));

	
}

void APlayerCharacter::MoveForward(const float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}


void APlayerCharacter::MoveRight(const float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void APlayerCharacter::Jump()
{
	if(bIsDashing || bIsJumping)
		return;
	Super::Jump();

	bIsJumping = true;
}

void APlayerCharacter::Landed(const FHitResult & Hit)
{
	bIsJumping = false;
}

void APlayerCharacter::SetAge(const FPlayerAge newAge)
{
	Age = newAge;

	CapsuleComp->SetCapsuleHalfHeight(Age.CapsuleHeight / 2);
	CapsuleComp->SetCapsuleRadius(Age.CapsuleRadius);

	BaseEyeHeight = Age.CapsuleHeight * 0.5 - 20;
	CrouchedEyeHeight = Age.CapsuleHeight * 0.5 * 0.6;
}

FVector APlayerCharacter::GetHandlePosition() const
{
	FVector cameraPosition;
	FRotator cameraRotator;
	GetActorEyesViewPoint(cameraPosition, cameraRotator);
	const FVector cameraForward = cameraRotator.Vector();
	const FVector handlePosition = cameraPosition + cameraForward * (HandleDistance + CapsuleComp->GetScaledCapsuleRadius());

	return handlePosition;
}

