// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameWidget.h"
#include "GameHUD.h"
#include "StaticMeshSorter.h"
#include "GameFramework/PlayerController.h"

#define LOCTEXT_NAMESPACE "Game"

void SGameWidget::Construct(const FArguments& InArgs) {

    bCanSupportFocus = true;
    OwningHUD = InArgs._OwningHUD;

    const FMargin ContentPadding = FMargin(100.f, 100.f);
    const FMargin ButtonPadding = FMargin(10.f);
    const FText SortText = LOCTEXT("SortText", "Sort");
    const FText ResetText = LOCTEXT("ResetText", "Reset");

    FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
    ButtonTextStyle.Size = 20.f;

    ChildSlot [
        SNew(SOverlay)
        + SOverlay::Slot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Bottom)
        .Padding(ContentPadding) [

            SNew(SVerticalBox)

            // sort button
            + SVerticalBox::Slot()
            .Padding(ButtonPadding) [
                SNew(SButton)
                .OnClicked(this, &SGameWidget::OnSortClicked) [
                    SNew(STextBlock)
                    .Font(ButtonTextStyle)
                    .Text(SortText)
                    .Justification(ETextJustify::Center)
                ]
            ]
        ]
        + SOverlay::Slot()
        .HAlign(HAlign_Right)
        .VAlign(VAlign_Bottom)
        .Padding(ContentPadding) [

            SNew(SVerticalBox)

            // sort button
            + SVerticalBox::Slot()
            .Padding(ButtonPadding) [
                SNew(SButton)
                .OnClicked(this, &SGameWidget::OnResetClicked) [
                    SNew(STextBlock)
                    .Font(ButtonTextStyle)
                    .Text(ResetText)
                    .Justification(ETextJustify::Center)
                ]
            ]
        ]
    ];

}

FReply SGameWidget::OnSortClicked() const {
    AStaticMeshSorter* gm = Cast<AStaticMeshSorter>(OwningHUD->PlayerOwner->GetWorld()->GetAuthGameMode());
    gm->Sort();
    return FReply::Handled();
}

FReply SGameWidget::OnResetClicked() const {    
    AStaticMeshSorter* gm = Cast<AStaticMeshSorter>(OwningHUD->PlayerOwner->GetWorld()->GetAuthGameMode());
    gm->Reset();
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
