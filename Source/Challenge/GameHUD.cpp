// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "SGameWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AGameHUD::BeginPlay() {
    Super::BeginPlay();

    if(GEngine && GEngine->GameViewport) {
        GameWidget = SNew(SGameWidget).OwningHUD(this);
        GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameWidgetContainer, SWeakWidget).PossiblyNullContent(GameWidget.ToSharedRef()));
    }
}

