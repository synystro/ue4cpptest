// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACustomActor::ACustomActor() {
	StartingPosition = { 0.f, 0.f, 0.f};
	NumberOfVertices = 0;  
}

void ACustomActor::BeginPlay() {

	Super::BeginPlay();

  // get number of vertices from this actor's static mesh
  ExtractNumberOfVertices();	

  // log number of vertices
  UE_LOG(LogTemp, Warning, TEXT("%s number of vertices: %d"), *this->GetName(), NumberOfVertices);	
	
	StartingPosition = GetActorLocation();

}

void ACustomActor::ExtractNumberOfVertices() {
  TArray<UStaticMeshComponent*> Comps;
  this->GetComponents(Comps);
  if (Comps.Num() > 0) {
    UStaticMeshComponent* StaticMeshComponent = Comps[0];
    if (!IsValidLowLevel()) return;
    if (!StaticMeshComponent) return;
    if (!StaticMeshComponent->GetStaticMesh()) return;
    if (!StaticMeshComponent->GetStaticMesh()->RenderData) return;

    UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();

    if (StaticMesh->RenderData->LODResources.Num() > 0) {
      FPositionVertexBuffer* VertexBuffer =
          &StaticMesh->RenderData->LODResources[0]
               .VertexBuffers.PositionVertexBuffer;
      if (VertexBuffer) {
        NumberOfVertices = VertexBuffer->GetNumVertices();
      }
    }

  } else {
    UE_LOG(LogTemp, Warning, TEXT("Actor %s has no static mesh component!"),
    *this->GetName());
  }
}
