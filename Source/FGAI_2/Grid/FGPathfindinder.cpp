// Fill out your copyright notice in the Description page of Project Settings.


#include "FGPathfindinder.h"

#include "Components/DrawSphereComponent.h"


// Sets default values
AFGPathfindinder::AFGPathfindinder(){
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DrawSphereComponent = CreateDefaultSubobject<UDrawSphereComponent>(TEXT("Sphere"));
	RootComponent = DrawSphereComponent;
	DrawSphereComponent->InitSphereRadius(75.f);
	DrawSphereComponent->ShapeColor = FColor::Black;
}

// Called when the game starts or when spawned
void AFGPathfindinder::BeginPlay(){
	Super::BeginPlay();


}

// Called every frame
void AFGPathfindinder::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
	if ( Path.Num() > 0 ){
		FVector CurrentLocation = GetActorLocation();
		if ( LastStep == FVector::ZeroVector ){
			LastStep = CurrentLocation;
			TargetLoc = Path.Last();
			if ( Path.Num() > 0 ){
				NextStep = Path[0];
			}
			else{
				NextStep = TargetLoc;
			}
		}

		if ( CurrentLocation.Equals(TargetLoc, Tolerance) ){
			Path.Empty();
			Destroy();
			return;
		}

		if ( CurrentLocation.Equals(NextStep, Tolerance) ){
			LastStep = NextStep;
			Path.RemoveSingle(NextStep);
			Path.Shrink();
			NextStep = Path[0];
		}
		if ( Path.Num() > 0 ){
			FVector DeltaPosition = FMath::Lerp(CurrentLocation, NextStep, MoveSpeed * DeltaTime);
			SetActorLocation(DeltaPosition);
		}
	}
}
