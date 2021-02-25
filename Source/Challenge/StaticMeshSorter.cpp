// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticMeshSorter.h"
#include "Algo/Reverse.h"
#include "GameHUD.h"
#include "Kismet/GameplayStatics.h"

// constructor
AStaticMeshSorter::AStaticMeshSorter() {
  StartingPosition = {0.f,0.f,0.f};
  OffsetPosition = {0.f, -300.f, 0.f};
  FindMovableCustomActorsOnLevel();
  HUDClass = AGameHUD::StaticClass();
}
// begin play
void AStaticMeshSorter::BeginPlay() {
  Super::BeginPlay();

  // log number of custom actors found in the map
  UE_LOG(LogTemp, Warning, TEXT("Total found custom movable actors: %d"),
         FoundCustomActors.Num());

  // create a map with the custom actors and their respective number of vertices
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

    // log all custom actor vertices to the output log
    LogCustomActorVertices();

    // (auto) sort actors on begin play
    //Sort();
  } else {
    UE_LOG(LogTemp, Warning, TEXT("No custom movable actors found in the map."));
  }

}
void AStaticMeshSorter::LogCustomActorVertices() {
   TMap<ACustomActor*, int> tempActorVertexMap = ActorVertexMap;
   tempActorVertexMap.ValueSort([](int A, int B) { return A > B; });
   for(auto& tempActorVertexPair : tempActorVertexMap) {
     UE_LOG(LogTemp, Warning, TEXT("%d is the number of vertices of %s's SM."), tempActorVertexPair.Value, *tempActorVertexPair.Key->GetName());
   }
}
// sorts all found (movable) custom actors according to their number of vertices
void AStaticMeshSorter::Sort() {
  // extract positions to an array for later comparison
  TArray<FVector> worldPositions;
  PositionMap.GenerateKeyArray(worldPositions);
  // loop through all actors and sort their position by number of vertices when they match with one of the sorted array of vertices elements
  for (auto& ActorVertexPair : ActorVertexMap) {
    bool repositioned = false;
    for (int i = 0; i < VertexValues.Num(); i++) {
      if(repositioned) break;
      // if custom actor's number of vertices matches the sorted array's vertices value
      if (ActorVertexPair.Value == VertexValues[i]) {
        for(auto& PositionPair : PositionMap) {
          if(PositionPair.Key == worldPositions[i] && PositionPair.Value == false) {
            ActorVertexPair.Key->SetTargetPosition(PositionPair.Key);
            PositionPair.Value = true;
            repositioned = true;
            break;
          }
        }
      }
    }
  }
}
// resets all found (movable) custom actors location to their starting position
void AStaticMeshSorter::Reset() {
  // reset actor locations
  for(auto& Elem : FoundCustomActors)
    Elem->SetTargetPosition(Elem->GetStartingPosition());
  // reset location occupied
  for(auto& Elem : PositionMap)
    Elem.Value = false;
}
// loop through all actors, check who is a custom actor and, if they are movable, add them to their corresponding tarray (FoundCustomActors)
void AStaticMeshSorter::FindMovableCustomActorsOnLevel() {
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomActor::StaticClass(),
                                        FoundActors);

  for (AActor* TActor : FoundActors) {
    ACustomActor* CustomActor = Cast<ACustomActor>(TActor);

    if (CustomActor != nullptr) {
      UStaticMeshComponent* StaticMeshComp = GetStaticMeshOnActor(CustomActor);
      if (StaticMeshComp != nullptr) {
        if(StaticMeshComp->Mobility == EComponentMobility::Movable)
          FoundCustomActors.Add(CustomActor);
      }
    }
  }
}
// gets static mesh from this actor
UStaticMeshComponent* AStaticMeshSorter::GetStaticMeshOnActor(AActor* Actor) {
  TArray<UStaticMeshComponent*> Comps;

  Actor->GetComponents(Comps);
  if (Comps.Num() > 0) {
    UStaticMeshComponent* StaticMeshComp = Comps[0];
    return StaticMeshComp;
  }

  return nullptr;
}
// quick sort algorithm
void AStaticMeshSorter::QuickSort(TArray<int>& tarr, int s, int e) {
  if (s < e) {
    int p = Partition(tarr, s, e);
    QuickSort(tarr, s, (p - 1));
    QuickSort(tarr, (p + 1), e);
  }
}
// part of quick sort algorithm
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