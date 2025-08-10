// Fill out your copyright notice in the Description page of Project Settings.

#include "PanelPrincipal.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "HUD_terror.h"

void UPanelPrincipal::NativeConstruct()
{
    Super::NativeConstruct();
    if (B_comenzar) B_comenzar->OnClicked.AddDynamic(this, &UPanelPrincipal::OnComenzar);
    if (B_Salir)    B_Salir->OnClicked.AddDynamic(this, &UPanelPrincipal::OnSalir);
}

void UPanelPrincipal::OnComenzar()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AHUD_terror* Hud = PC->GetHUD<AHUD_terror>())
        {
            Hud->OcultarPanelPrincipal();
            Hud->MostrarHUD();
        }
        RemoveFromParent();
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
        PC->SetPause(false);
    }
}

void UPanelPrincipal::OnSalir()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
}

void UPanelPrincipal::Mostrar()
{
    if (!IsInViewport()) AddToViewport();
}

void UPanelPrincipal::Cerrar()
{
    if (IsInViewport()) RemoveFromParent();
}
