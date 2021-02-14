// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticMeshSorter.h"
#include "Algo/Reverse.h"
#include "GameHUD.h"
#include "Kismet/GameplayStatics.h"


AStaticMeshSorter::AStaticMeshSorter() {
  FindMovableActorsOnLevel();
  HUDClass = AGameHUD::StaticClass();
}

void AStaticMeshSorter::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Warning, TEXT("Total found custom movable actors: %d"),
         FoundCustomActors.Num());

  if (FoundCustomActors.Num() > 0) {
    for (auto& Elem : FoundCustomActors) {
      VertexValues.Add(Elem->GetTotalVertices());
      PositionMap.Add(Elem->GetStartingPosition(), false);

      ActorVertexMap.Add(Elem, Elem->GetTotalVertices());
      //UE_LOG(LogTemp, Warning, TEXT("%s's number of vertices: %d"), *Elem->GetName(), Elem->GetTotalVertices());
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
    // sort actors on begin play
    //Sort();
  }
}

void AStaticMeshSorter::Sort() {
  // generate positions array and reverse it
  TArray<FVector> worldPositions;
  PositionMap.GenerateKeyArray(worldPositions);
  Algo::Reverse(worldPositions); 
  // loop through all actors and sort them by their sorted vertex when they match
  for (auto& ActorVertexPair : ActorVertexMap) {
    bool repositioned = false;
    for (int i = 0; i < VertexValues.Num(); i++) {
      // if custom actor repositioned, break to loop the next custom actor
      if (repositioned) break;
      // if custom actor's number of vertices matches i's vertices value
      if (ActorVertexPair.Value == VertexValues[i]) {
        for (auto& PositionPair : PositionMap) {
          if (PositionPair.Key == worldPositions[i] && PositionPair.Value == false) {
            ActorVertexPair.Key->SetActorLocation(PositionPair.Key);
            PositionPair.Value = true;
            repositioned = true;            
            break;
          }
        }
        continue;
      }
    }
  }
}

void AStaticMeshSorter::Reset() {
  // reset actor locations
  for(auto& Elem : FoundCustomActors) {
    Elem->SetActorLocation(Elem->GetStartingPosition());
  }
  // reset location occupied
  for(auto& Elem : PositionMap) {
    Elem.Value = false;
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