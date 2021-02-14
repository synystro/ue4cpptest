// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 *
 */
class SGameWidget : public SCompoundWidget {
 public:
  SLATE_BEGIN_ARGS(SGameWidget) {}
  SLATE_ARGUMENT(TWeakObjectPtr<class AGameHUD>, OwningHUD)
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

  FReply OnSortClicked() const;
  FReply OnResetClicked() const;

  TWeakObjectPtr<class AGameHUD> OwningHUD;

  virtual bool SupportsKeyboardFocus() const override { return true; };
};
