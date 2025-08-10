
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_terror.generated.h"


class UGameTerror;
class UNotascpp;
class UCajaFuertecpp;
class ACajaFuerte;
class UpiramidePuzzle;
class UPanelPrincipal;

UCLASS()
class GAME_TERRO_WGJ_API AHUD_terror : public AHUD
{
	GENERATED_BODY()

public:
	AHUD_terror();
	virtual void BeginPlay() override;

    void MostrarHUD();
    void OcultarHUD();

    void EstablecerCansancio(float valor);
    void EstablecerInteraccionVisible(bool visible);
    void EstablecerSlot(int32 indice, UTexture2D* icono);
    void LimpiarSlots();

    void MostrarHistoria(const FText& texto);
    void OcultarHistoria();

    void MostrarCajaFuerte(ACajaFuerte* owner, const FString& codigo);
    void OcultarCajaFuerte();

public:
    UClass* ClaseHUD = nullptr;
    UPROPERTY() UGameTerror* HUDWidget = nullptr;


    UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UNotascpp> NotasClass;
    UPROPERTY() UNotascpp* NotasWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UCajaFuertecpp> ClaseCajaWidget;
    UPROPERTY() UCajaFuertecpp* CajaWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UpiramidePuzzle> ClasePiramideWidget;

    UPROPERTY() UpiramidePuzzle* PiramideWidget = nullptr;

public:
    // ...
    void MostrarPistaPiramide(UTexture2D* Texture, int32 Digit);
    void OcultarPistaPiramide();

    void MostrarPanelPrincipal();
    void OcultarPanelPrincipal();

    UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UPanelPrincipal> ClasePanelPrincipal;

    UPROPERTY() UPanelPrincipal* PanelPrincipalWidget = nullptr;

};
