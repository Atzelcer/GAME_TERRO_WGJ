// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "CajaFuertecpp.generated.h"

class ACajaFuerte;

UCLASS()
class GAME_TERRO_WGJ_API UCajaFuertecpp : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget)) UButton* ButtonDescifrar;
    UPROPERTY(meta = (BindWidget)) UEditableTextBox* codeDes;

    UFUNCTION(BlueprintCallable) void Setup(ACajaFuerte* owner, const FString& expected);

protected:
    virtual void NativeConstruct() override;

    UFUNCTION() void onTextChanged(const FText& text);
    UFUNCTION() void onClickDescifrar();

private:
    TWeakObjectPtr<ACajaFuerte> ownerRef;
    FString expectedCode;
    FString currentCode;
};
