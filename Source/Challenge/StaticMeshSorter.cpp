// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticMeshSorter.h"
#include "Algo/Reverse.h"
#include "GameHUD.h"
#include "Kismet/GameplayStatics.h"


AStaticMeshSorter::AStaticMeshSorter() {
  StartingPosition = {0.f,0.f,0.f};
  OffsetPosition = {0.f, -300.f, 0.f};
  FindMovableActorsOnLevel();
  HUDClass = AGameHUD::StaticClass();
}

void AStaticMeshSorter::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Warning, TEXT("Total found custom movable actors: %d"),
         FoundCustomActors.Num());

  if (FoundCustomActors.Num() > 0) {
    for (auto& CustomActor : FoundCustomActors) {
      VertexValues.Add(CustomActor->GetTotalVertices());
      ActorVertexMap.Add(CustomActor, CustomActor->GetTotalVertices());
    }

    // sort vertices by value
    QuickSort(VertexValues, 0, VertexValues.Num() - 1);

    // create a position map
    for(int i = 0; i < FoundCustomActors.Num(); i++) {      
      PositionMap.Add(StartingPosition + (OffsetPosition * i), false);
    }

    // (auto) sort actors on begin play
    //Sort();
  }

}

void AStaticMeshSorter::Sort() {
  // extract positions to an array for later comparison
  TArray<FVector> worldPositions;
  PositionMap.GenerateKeyArray(worldPositions);
  // loop through all actors and sort them by vertex when a NumberOfVertices match is found
  for (auto& ActorVertexPair : ActorVertexMap) {
    bool repositioned = false;
    for (int i = 0; i < VertexValues.Num(); i++) {
      if(repositioned) break;
      // if custom actor's number of vertices matches i's vertices value
      if (ActorVertexPair.Value == VertexValues[i]) {
        for(auto& PositionPair : PositionMap) {
          if(PositionPair.Key == worldPositions[i] && PositionPair.Value == false) {
            ActorVertexPair.Key->SetActorLocation(PositionPair.Key);
            PositionPair.Value = true;
            repositioned = true;
            break;
          }
        }
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