// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAME_TERRO_WGJGameMode.generated.h"

UCLASS(minimalapi)
class AGAME_TERRO_WGJGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAME_TERRO_WGJGameMode();

	virtual void BeginPlay() override;

	class AHistoriasActivador* activador = nullptr;
};



