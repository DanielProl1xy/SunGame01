// Fill out your copyright notice in the Description page of Project Settings.


#include "Game01/Public/PlayerControlller.h"

APlayerControlller::APlayerControlller()
{
	
}

void APlayerControlller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(FName("Pitch"), this, &APlayerControlller::PitchAtRate);
	InputComponent->BindAxis(FName("Yaw"), this, &APlayerControlller::YawAtRate);
}

void APlayerControlller::PitchAtRate(float rate)
{
	AddPitchInput(-rate);
}

void APlayerControlller::YawAtRate(float rate)
{
	AddYawInput(rate);
}