// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "FGPathfindinder.generated.h"

class UDrawSphereComponent;


UCLASS()
class FGAI_2_API AFGPathfindinder : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFGPathfindinder();
	UPROPERTY()
	UDrawSphereComponent* DrawSphereComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Path;
	UPROPERTY(VisibleAnywhere)
	FVector LastStep = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	FVector NextStep = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	FVector TargetLoc = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	float Tolerance = 250.f;
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 1.f;
	FRichCurve XCurve;
	FRichCurve YCurve;
	float TravelTime;
};
