#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTerror.generated.h"

class UImage;
class UButton;
class UProgressBar;
class UWidget;
class UTexture2D;

UCLASS()
class GAME_TERRO_WGJ_API UGameTerror : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Funciones esenciales
	UFUNCTION(BlueprintCallable, Category = "HUD") void EstablecerCansancio(float valor);
	UFUNCTION(BlueprintCallable, Category = "HUD") void EstablecerInteraccionVisible(bool visible);
	UFUNCTION(BlueprintCallable, Category = "HUD") void EstablecerSlotRapido(int32 indice, UTexture2D* icono);
	UFUNCTION(BlueprintCallable, Category = "HUD") void LimpiarSlotsRapidos();

private:
	UButton* Button_menu = nullptr;
	UWidget* RecogerObjeto = nullptr;
	UImage* inv1 = nullptr;
	UImage* inv2 = nullptr;
	UImage* inv3 = nullptr;
	UImage* inv4 = nullptr;
	UProgressBar* BarraAgotamiento = nullptr;

	UImage* ObtenerSlotPorIndice(int32 indice) const;

	float valorCansancioGuardado = 0.f;
	bool  interaccionVisibleGuardada = false;
};
