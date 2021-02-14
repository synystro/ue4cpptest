// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomActor.generated.h"

UCLASS()
class CHALLENGE_API ACustomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomActor();
	FVector GetStartingPosition() { return StartingPosition; } ;
	int GetTotalVertices() { return NumberOfVertices; } ;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ExtractNumberOfVertices();

	FVector StartingPosition;
	int NumberOfVertices;

};
