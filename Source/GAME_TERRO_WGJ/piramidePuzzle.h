// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "piramidePuzzle.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;

UCLASS()
class GAME_TERRO_WGJ_API UpiramidePuzzle : public UUserWidget
{
    GENERATED_BODY()

public:
    // deben llamarse igual que en el BP
    UPROPERTY(meta = (BindWidget)) UImage* AcertijoImage = nullptr;
    UPROPERTY(meta = (BindWidget)) UTextBlock* codeNumber = nullptr;

    UFUNCTION(BlueprintCallable) void Setup(UTexture2D* InTexture, int32 InDigit);

protected:
    virtual void NativeConstruct() override;
};