#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/GameViewportClient.h"

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();

    // Ekranın boyutunu al
    FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    // Crosshair'ın boyutlarını ve orta noktasını hesapla
    float CrosshairSize = 10.f;
    FVector2D CrosshairPosition(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

    // Crosshair'ın iç kısmını temsil eden boyutları hesapla
    float InnerCrosshairSize = CrosshairSize * 0.5f;
    float InnerCrosshairOffset = InnerCrosshairSize * 0.5f;

    // Crosshair'ı çizmek için kullanılacak çizgi kalınlığı
    float LineThickness = 2.f;

    // Crosshair'ın dış kısmını yatay olarak çiz
    DrawLine(
        CrosshairPosition.X - CrosshairSize,
        CrosshairPosition.Y,
        CrosshairPosition.X + CrosshairSize,
        CrosshairPosition.Y,
        FLinearColor::White,
        LineThickness
    );

    // Crosshair'ın dış kısmını dikey olarak çiz
    DrawLine(
        CrosshairPosition.X,
        CrosshairPosition.Y - CrosshairSize,
        CrosshairPosition.X,
        CrosshairPosition.Y + CrosshairSize,
        FLinearColor::White,
        LineThickness
    );

    // Crosshair'ın iç kısmını temsil eden yatay çizgiyi çiz
    DrawLine(
        CrosshairPosition.X - InnerCrosshairSize - InnerCrosshairOffset,
        CrosshairPosition.Y,
        CrosshairPosition.X + InnerCrosshairSize - InnerCrosshairOffset,
        CrosshairPosition.Y,
        FLinearColor::Black,
        LineThickness
    );

    // Crosshair'ın iç kısmını temsil eden dikey çizgiyi çiz
    DrawLine(
        CrosshairPosition.X,
        CrosshairPosition.Y - InnerCrosshairSize - InnerCrosshairOffset,
        CrosshairPosition.X,
        CrosshairPosition.Y + InnerCrosshairSize - InnerCrosshairOffset,
        FLinearColor::Black,
        LineThickness
    );
}
