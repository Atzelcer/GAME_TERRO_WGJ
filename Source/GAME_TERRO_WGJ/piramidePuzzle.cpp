// Fill out your copyright notice in the Description page of Project Settings.


#include "piramidePuzzle.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UpiramidePuzzle::NativeConstruct()
{
    Super::NativeConstruct();
}

void UpiramidePuzzle::Setup(UTexture2D* InTexture, int32 InDigit)
{
    if (AcertijoImage)
    {
        if (InTexture)
        {
            // más directo y seguro
            AcertijoImage->SetBrushFromTexture(InTexture, /*bMatchSize=*/false);
            AcertijoImage->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            AcertijoImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (codeNumber)
    {
        codeNumber->SetText(FText::AsNumber(InDigit));
        // opcional: si no ves el número, revisa color/alpha en el BP
        // codeNumber->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
    }
}
