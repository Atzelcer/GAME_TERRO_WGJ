// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Notascpp.generated.h"


UCLASS()
class GAME_TERRO_WGJ_API UNotascpp : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

    class URichTextBlock* RelatoP;

    void SetHistoria(const FText& markup);

};