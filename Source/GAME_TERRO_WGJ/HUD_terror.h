
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_terror.generated.h"

class UGameTerror;
class UNotascpp;
class UTexture2D;

UCLASS()
class GAME_TERRO_WGJ_API AHUD_terror : public AHUD
{
	GENERATED_BODY()

public:
	AHUD_terror();
	virtual void BeginPlay() override;

    void MostrarHUD();
    void OcultarHUD();

    // puente HUD principal
    void EstablecerCansancio(float valor);
    void EstablecerInteraccionVisible(bool visible);
    void EstablecerSlot(int32 indice, UTexture2D* icono);
    void LimpiarSlots();

    // historias (mostrar/ocultar via HUD)
    void MostrarHistoria(const FText& texto);
    void OcultarHistoria();

private:
    UClass* ClaseHUD = nullptr;
    UGameTerror* HUDWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="Historia")
    TSubclassOf<UNotascpp> NotasClass;

    UPROPERTY() UNotascpp* NotasWidget = nullptr;
};