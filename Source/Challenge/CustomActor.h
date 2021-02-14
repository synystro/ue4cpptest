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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	
	void ExtractNumberOfVertices();

	FVector StartingPosition;
	FVector TargetPosition;
	int NumberOfVertices;

	// lerp
	FVector LerpStartPosition;
	float LerpTime = 0.f;
	const float LerpDuration = 1.f;

public:
	FVector GetStartingPosition() { return StartingPosition; } ;
	void SetTargetPosition(FVector targetPosition) {
		//TODO: put this in a delegate function later
		LerpStartPosition = GetActorLocation();
		LerpTime = 0.f;
		TargetPosition = targetPosition;
		//
		SetActorTickEnabled(true);
	};
	int GetTotalVertices() { return NumberOfVertices; } ;

};
