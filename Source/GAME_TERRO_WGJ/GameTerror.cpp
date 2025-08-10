
#include "GameTerror.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UGameTerror::NativeConstruct()
{
	Super::NativeConstruct();

	Button_menu = Cast<UButton>(GetWidgetFromName(TEXT("Button_menu")));
	RecogerObjeto = Cast<UWidget>(GetWidgetFromName(TEXT("RecogerObjeto")));
	inv1 = Cast<UImage>(GetWidgetFromName(TEXT("inv1")));
	inv2 = Cast<UImage>(GetWidgetFromName(TEXT("inv2")));
	inv3 = Cast<UImage>(GetWidgetFromName(TEXT("inv3")));
	inv4 = Cast<UImage>(GetWidgetFromName(TEXT("inv4")));
	BarraAgotamiento = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar-agotamiento")));

	EstablecerCansancio(valorCansancioGuardado);
	EstablecerInteraccionVisible(interaccionVisibleGuardada);
}

void UGameTerror::EstablecerCansancio(float valor)
{
	valorCansancioGuardado = FMath::Clamp(valor, 0.f, 1.f);
	if (BarraAgotamiento)
	{
		BarraAgotamiento->SetPercent(valorCansancioGuardado);
	}
}

void UGameTerror::EstablecerInteraccionVisible(bool visible)
{
	interaccionVisibleGuardada = visible;
	if (RecogerObjeto)
	{
		RecogerObjeto->SetVisibility(visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

UImage* UGameTerror::ObtenerSlotPorIndice(int32 indice) const
{
	switch (indice)
	{
	case 0: return inv1;
	case 1: return inv2;
	case 2: return inv3;
	case 3: return inv4;
	default: return nullptr;
	}
}

void UGameTerror::EstablecerSlotRapido(int32 indice, UTexture2D* icono)
{
	if (UImage* S = ObtenerSlotPorIndice(indice))
	{
		S->SetBrushFromTexture(icono);
	}
}

void UGameTerror::LimpiarSlotsRapidos()
{
	for (int32 i = 0; i < 4; ++i)
	{
		EstablecerSlotRapido(i, nullptr);
	}
}
