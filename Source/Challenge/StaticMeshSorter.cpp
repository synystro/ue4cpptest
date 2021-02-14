// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticMeshSorter.h"

#include "Algo/Reverse.h"
#include "Kismet/GameplayStatics.h"

AStaticMeshSorter::AStaticMeshSorter() {
  // SortStartPosition = {0.f,0.f,0.f};
  // SortSideOffset = {0.f, 100.f, 0.f};
  FindMovableActorsOnLevel();
}

void AStaticMeshSorter::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Warning, TEXT("Found custom movable actors: %d"),
         FoundCustomActors.Num());

  if (FoundCustomActors.Num() > 0) {
    for (auto& Elem : FoundCustomActors) {
      VertexValues.Add(Elem->GetTotalVertices());
      PositionMap.Add(Elem->GetStartingPosition(), false);

      ActorVertexMap.Add(Elem, Elem->GetTotalVertices());
      UE_LOG(LogTemp, Warning, TEXT("%s's vertices %d"), *Elem->GetName(),
             Elem->GetTotalVertices());
    }

    // sort vertices
    QuickSort(VertexValues, 0, VertexValues.Num() - 1);

    // sort position's based on Y axis
    TArray<int> yPositions;
    for (auto& pos : PositionMap) yPositions.Add(pos.Key.Y);
    QuickSort(yPositions, 0, PositionMap.Num() - 1);

    int i = 0;
    for (auto& pos : PositionMap) {
      pos.Key.Y = yPositions[i];
      i++;
    }

    // generate positions array and reverse it
    TArray<FVector> worldPositions;
    PositionMap.GenerateKeyArray(worldPositions);
    Algo::Reverse(worldPositions);

    // sort actors

    for (auto& Elem : ActorVertexMap) {
      bool repositioned = false;
      for (int j = 0; j < VertexValues.Num(); j++) {
        if (repositioned) break;
        if (Elem.Value == VertexValues[j]) {
          for (auto& Elem2 : PositionMap) {
            if (Elem2.Key == worldPositions[j] && Elem2.Value == false) {
              Elem.Key->SetActorLocation(Elem2.Key);
              Elem2.Value = true;
              repositioned = true;
              break;
            }
          }
          continue;
        }
      }
    }
  }
}

void AStaticMeshSorter::FindMovableActorsOnLevel() {
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomActor::StaticClass(),
                                        FoundActors);

  for (AActor* TActor : FoundActors) {
    ACustomActor* CustomActor = Cast<ACustomActor>(TActor);

    if (CustomActor != nullptr) {
      if (CheckForStaticMeshOnActor(CustomActor)) {
        FoundCustomActors.Add(CustomActor);
      }
    }
  }
}

bool AStaticMeshSorter::CheckForStaticMeshOnActor(AActor* Actor) {
  TArray<UStaticMeshComponent*> Comps;

  Actor->GetComponents(Comps);
  if (Comps.Num() > 0) {
    UStaticMeshComponent* FoundComp = Comps[0];
    if (FoundComp->Mobility == EComponentMobility::Movable) return true;
  }

  return false;
}

void AStaticMeshSorter::QuickSort(TArray<int>& tarr, int s, int e) {
  if (s < e) {
    int p = Partition(tarr, s, e);
    QuickSort(tarr, s, (p - 1));
    QuickSort(tarr, (p + 1), e);
  }
}

int AStaticMeshSorter::Partition(TArray<int>& tarr, int s, int e) {
  int pivot = tarr[e];  // end pivot index
  int sIndex = s;       // smaller n index

  for (int i = s; i < e; i++) {
    if (tarr[i] < pivot) {
      int temp = tarr[i];
      tarr[i] = tarr[sIndex];
      tarr[sIndex] = temp;
      sIndex++;
    }
  }
  int temp = tarr[e];
  tarr[e] = tarr[sIndex];
  tarr[sIndex] = temp;

  return sIndex;
}