// Fill out your copyright notice in the Description page of Project Settings.

#include "CajaFuertecpp.h"
#include "CajaFuerte.h"

static FString digitsOnlyMax4(const FString& in)
{
    FString out; out.Reserve(4);
    for (TCHAR c : in) if (FChar::IsDigit(c)) { out.AppendChar(c); if (out.Len() == 4) break; }
    return out;
}

void UCajaFuertecpp::Setup(ACajaFuerte* owner, const FString& expected)
{
    ownerRef = owner;
    expectedCode = expected;
}

void UCajaFuertecpp::NativeConstruct()
{
    Super::NativeConstruct();
    if (codeDes)
    {
        codeDes->SetIsReadOnly(false);
        codeDes->SetKeyboardFocus();   
    }

    if (codeDes)      codeDes->OnTextChanged.AddDynamic(this, &UCajaFuertecpp::onTextChanged);
    if (ButtonDescifrar) ButtonDescifrar->OnClicked.AddDynamic(this, &UCajaFuertecpp::onClickDescifrar);
}

void UCajaFuertecpp::onTextChanged(const FText& text)
{
    const FString f = digitsOnlyMax4(text.ToString());
    if (codeDes && f != text.ToString()) codeDes->SetText(FText::FromString(f));
    currentCode = f;
}

void UCajaFuertecpp::onClickDescifrar()
{
    ownerRef->AbrirCajaFuerte();
    //if (!ownerRef.IsValid()) return;
    //const bool ok = (currentCode.Len() == 4 && currentCode == expectedCode);
    //if (ok) ownerRef->AbrirCajaFuerte();    // accion en el actor
    //else    ownerRef->FeedbackCodigoIncorrecto(); // opcional (parpadeo, sonido)
}
