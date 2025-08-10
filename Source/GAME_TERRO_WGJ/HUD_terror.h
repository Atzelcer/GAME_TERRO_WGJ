
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_terror.generated.h"

class UGameTerror;
class UTexture2D;

UCLASS()
class GAME_TERRO_WGJ_API AHUD_terror : public AHUD
{
	GENERATED_BODY()

public:
	AHUD_terror();
	virtual void BeginPlay() override;

	// Mostrar / Ocultar
	void MostrarHUD();
	void OcultarHUD();

	// Puente hacia el widget
	void EstablecerCansancio(float valor);
	void EstablecerInteraccionVisible(bool visible);
	void EstablecerSlot(int32 indice, UTexture2D* icono);
	void LimpiarSlots();

private:
	UClass* ClaseHUD = nullptr;      // BP que hereda de UGameTerror (asignado por FClassFinder)
	UGameTerror* HUDWidget = nullptr; // Instancia creada en BeginPlay
};