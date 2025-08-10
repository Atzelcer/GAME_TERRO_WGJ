// Fill out your copyright notice in the Description page of Project Settings.


#include "Notascpp.h"

void UNotascpp::NativeConstruct()
{
    Super::NativeConstruct();

    if (RelatoP) RelatoP->SetAutoWrapText(true);
}

void UNotascpp::SetHistoria(const FText& markup)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Llego la historia"));
    if (RelatoP) RelatoP->SetText(markup);
}
