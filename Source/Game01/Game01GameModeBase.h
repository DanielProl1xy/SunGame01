// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Public/Settings/PlayerAge.h"
#include "Game01GameModeBase.generated.h"


UCLASS(Config=Game)
class GAME01_API AGame01GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AGame01GameModeBase();
};
