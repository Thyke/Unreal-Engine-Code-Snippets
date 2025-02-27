// Copyright (C) Thyke. All Rights Reserved.


#include "CrosshairHUD.h"
#include "Engine/Canvas.h"
#include "Engine/GameViewportClient.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CrosshairHUD)

void ACrosshairHUD::DrawHUD()
{
    Super::DrawHUD();

    // Get the screen size
    FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    // Calculate crosshair dimensions and center position
    float CrosshairSize = 10.f;
    FVector2D CrosshairPosition(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

    // Calculate dimensions for the inner part of the crosshair
    float InnerCrosshairSize = CrosshairSize * 0.5f;
    float InnerCrosshairOffset = InnerCrosshairSize * 0.5f;

    // Line thickness used for drawing the crosshair
    float LineThickness = 2.f;

    // Draw the outer part of the crosshair horizontally
    DrawLine(
       CrosshairPosition.X - CrosshairSize,
       CrosshairPosition.Y,
       CrosshairPosition.X + CrosshairSize,
       CrosshairPosition.Y,
       FLinearColor::White,
       LineThickness
    );

    // Draw the outer part of the crosshair vertically
    DrawLine(
       CrosshairPosition.X,
       CrosshairPosition.Y - CrosshairSize,
       CrosshairPosition.X,
       CrosshairPosition.Y + CrosshairSize,
       FLinearColor::White,
       LineThickness
    );

    // Draw the horizontal line representing the inner part of the crosshair
    DrawLine(
       CrosshairPosition.X - InnerCrosshairSize - InnerCrosshairOffset,
       CrosshairPosition.Y,
       CrosshairPosition.X + InnerCrosshairSize - InnerCrosshairOffset,
       CrosshairPosition.Y,
       FLinearColor::Black,
       LineThickness
    );

    // Draw the vertical line representing the inner part of the crosshair
    DrawLine(
       CrosshairPosition.X,
       CrosshairPosition.Y - InnerCrosshairSize - InnerCrosshairOffset,
       CrosshairPosition.X,
       CrosshairPosition.Y + InnerCrosshairSize - InnerCrosshairOffset,
       FLinearColor::Black,
       LineThickness
    );
}