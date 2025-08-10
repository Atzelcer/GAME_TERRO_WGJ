// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PanelPrincipal.generated.h"

class UButton;

UCLASS()
class GAME_TERRO_WGJ_API UPanelPrincipal : public UUserWidget
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget)) UButton* B_comenzar = nullptr;
    UPROPERTY(meta = (BindWidget)) UButton* B_Salir = nullptr;

    UFUNCTION() void OnComenzar();
    UFUNCTION() void OnSalir();

public:
    UFUNCTION(BlueprintCallable, Category = "UI") void Mostrar();
    UFUNCTION(BlueprintCallable, Category = "UI") void Cerrar();
};