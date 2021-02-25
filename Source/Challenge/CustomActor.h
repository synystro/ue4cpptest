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
	void OnSetTargetPosition();

	FVector StartingPosition;
	FVector TargetPosition;
	int NumberOfVertices;

	// UI
	TSharedPtr<class SActorWidget> ActorWidget;

	// lerp
	FVector LerpStartPosition;
	float LerpTimer = 0.f;
	const float LerpDuration = 1.f;

public:
	FVector GetStartingPosition() { return StartingPosition; } ;
	void SetTargetPosition(FVector targetPosition) {
		TargetPosition = targetPosition;
		OnSetTargetPosition();
	};
	int GetTotalVertices() { return NumberOfVertices; } ;

};
