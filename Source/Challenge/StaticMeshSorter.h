// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomActor.h"
#include "StaticMeshSorter.generated.h"

UCLASS()
class CHALLENGE_API AStaticMeshSorter : public AGameModeBase {
  GENERATED_BODY()

public:
void Sort();
void Reset();

 protected:
  AStaticMeshSorter();

  virtual void BeginPlay() override;

  void FindMovableActorsOnLevel();

  bool CheckForStaticMeshOnActor(AActor* Actor);

  void QuickSort(TArray<int>&, int s, int e);

  int Partition(TArray<int>&, int s, int e);

  FVector StartingPosition;
  FVector OffsetPosition;
  TMap<ACustomActor*, int> ActorVertexMap;
  TMap<FVector, bool> PositionMap;
  TArray<ACustomActor*> FoundCustomActors;
  TArray<int> VertexValues;
};
