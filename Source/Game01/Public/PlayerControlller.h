// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControlller.generated.h"


UCLASS()
class GAME01_API APlayerControlller : public APlayerController
{
	GENERATED_BODY()

public:

	APlayerControlller();

	void SetupInputComponent();

private:

	UFUNCTION()
	void PitchAtRate(float rate);

	UFUNCTION()
	void YawAtRate(float rate);
};
