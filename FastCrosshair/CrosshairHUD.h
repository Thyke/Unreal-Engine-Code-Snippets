// Copyright (C) Thyke. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()

public:
	// AHUD Interface
	virtual void DrawHUD() override;
};
