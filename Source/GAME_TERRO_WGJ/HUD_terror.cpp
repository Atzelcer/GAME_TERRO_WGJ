#include "HUD_terror.h"
#include "GameTerror.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AHUD_terror::AHUD_terror()
{
	static ConstructorHelpers::FClassFinder<UGameTerror> WBP(TEXT("/Game/HUD/PrimerPanel.PrimerPanel_C"));
	if (WBP.Succeeded())
	{
		ClaseHUD = WBP.Class;
	}
}

void AHUD_terror::BeginPlay()
{
	Super::BeginPlay();
	MostrarHUD();
}

void AHUD_terror::MostrarHUD()
{
	if (!ClaseHUD) return;

	if (!HUDWidget)
	{
		HUDWidget = CreateWidget<UGameTerror>(GetWorld(), ClaseHUD);
	}
	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
	}
}

void AHUD_terror::OcultarHUD()
{
	if (HUDWidget && HUDWidget->IsInViewport())
	{
		HUDWidget->RemoveFromParent();
	}
}

void AHUD_terror::EstablecerCansancio(float valor)
{
	if (HUDWidget) HUDWidget->EstablecerCansancio(valor);
}

void AHUD_terror::EstablecerInteraccionVisible(bool visible)
{
	if (HUDWidget) HUDWidget->EstablecerInteraccionVisible(visible);
}

void AHUD_terror::EstablecerSlot(int32 indice, UTexture2D* icono)
{
	if (HUDWidget) HUDWidget->EstablecerSlotRapido(indice, icono);
}

void AHUD_terror::LimpiarSlots()
{
	if (HUDWidget) HUDWidget->LimpiarSlotsRapidos();
}
