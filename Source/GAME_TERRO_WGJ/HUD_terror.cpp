#include "HUD_terror.h"
#include "GameTerror.h"
#include "Notascpp.h"
#include "CajaFuertecpp.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "piramidePuzzle.h"

AHUD_terror::AHUD_terror()
{
    static ConstructorHelpers::FClassFinder<UGameTerror> WBP(TEXT("/Game/HUD/PrimerPanel.PrimerPanel_C"));
    if (WBP.Succeeded()) ClaseHUD = WBP.Class;

    static ConstructorHelpers::FClassFinder<UNotascpp> WBPNotas(TEXT("/Game/HUD/Notas.Notas_C"));
    if (WBPNotas.Succeeded()) NotasClass = WBPNotas.Class;

    static ConstructorHelpers::FClassFinder<UCajaFuertecpp> WBPCaja(TEXT("/Game/HUD/W_CajaFuerte.W_CajaFuerte_C"));
    if (WBPCaja.Succeeded()) ClaseCajaWidget = WBPCaja.Class;

    static ConstructorHelpers::FClassFinder<UpiramidePuzzle> WPPira(TEXT("/Game/HUD/puzzleP.puzzleP"));
    if (WPPira.Succeeded()) ClasePiramideWidget = WPPira.Class;
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

void AHUD_terror::MostrarHistoria(const FText& texto)
{
	if (!NotasClass) return;

	if (!NotasWidget)
		NotasWidget = CreateWidget<UNotascpp>(GetOwningPlayerController(), NotasClass);
	if (!NotasWidget) return;

	NotasWidget->SetHistoria(texto);
	NotasWidget->AddToViewport(10);
}

void AHUD_terror::OcultarHistoria()
{
	if (NotasWidget)
		NotasWidget->RemoveFromParent();
}

void AHUD_terror::MostrarCajaFuerte(ACajaFuerte* owner, const FString& codigo)
{
    if (!ClaseCajaWidget) return;

    APlayerController* PC = GetOwningPlayerController();
    if (!PC) return;

    if (!CajaWidget)
        CajaWidget = CreateWidget<UCajaFuertecpp>(PC, ClaseCajaWidget);
    if (!CajaWidget) return;

    CajaWidget->Setup(owner, codigo);

    if (!CajaWidget->IsInViewport())
        CajaWidget->AddToViewport(70);

    PC->bShowMouseCursor = true;

    FInputModeUIOnly mode;
    mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    mode.SetWidgetToFocus(CajaWidget->TakeWidget()); 
    PC->SetInputMode(mode);

    PC->SetPause(true); 
}

void AHUD_terror::OcultarCajaFuerte()
{
    APlayerController* PC = GetOwningPlayerController();

    if (CajaWidget && CajaWidget->IsInViewport())
        CajaWidget->RemoveFromParent();

    if (PC)
    {
        PC->SetPause(false); 
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void AHUD_terror::MostrarPistaPiramide(UTexture2D* Texture, int32 Digit)
{
    if (!ClasePiramideWidget) return;

    if (!PiramideWidget)
        PiramideWidget = CreateWidget<UpiramidePuzzle>(GetOwningPlayerController(), ClasePiramideWidget);
    if (!PiramideWidget) return;

    PiramideWidget->Setup(Texture, Digit);
    if (!PiramideWidget->IsInViewport())
        PiramideWidget->AddToViewport(10);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,FString::Printf(TEXT("Entro a al hUDDDDDD mega")));
}

void AHUD_terror::OcultarPistaPiramide()
{
    if (PiramideWidget)
        PiramideWidget->RemoveFromParent();
}
