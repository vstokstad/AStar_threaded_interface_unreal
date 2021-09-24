// Fill out your copyright notice in the Description page of Project Settings.


#include "FGPathfindinder.h"
#include "Animation/AnimCurveTypes.h"
#include "Curves/RichCurve.h"
#include "Curves/CurveVector.h"
#include "Components/DrawSphereComponent.h"

#include "Kismet/KismetSystemLibrary.h"


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
	YCurve = FRichCurve();
	XCurve = FRichCurve();
	TravelTime = 0.f;
}

// Called every frame
void AFGPathfindinder::Tick( float DeltaTime ){
	Super::Tick(DeltaTime);
	if ( YCurve.GetNumKeys() > 0 && XCurve.GetNumKeys() > 0 ){

		NextStep = FVector(XCurve.Eval(TravelTime), YCurve.Eval(TravelTime), 10.f);

		Velocity += FVector(1.f, 1.f, 0.f) * DeltaTime;
		FVector NewLocation = FMath::Lerp(GetActorLocation(), NextStep, DeltaTime);
		FVector DeltaLocation = (NewLocation + Velocity * DeltaTime);
		SetActorLocation(DeltaLocation);
		UKismetSystemLibrary::DrawDebugLine(this,NextStep, DeltaLocation, FColor::Purple, TravelTime-Velocity.Size(), 10.f);
		TravelTime += (MoveSpeed * DeltaTime);

	}
	else{
		if ( Path.Num() > 0 ){
			for ( int i = 0; i < Path.Num(); ++i ){
				XCurve.AddKey(i, Path[i].X);
				YCurve.AddKey(i, Path[i].Y);
			}
			XCurve.AutoSetTangents(0);
			YCurve.AutoSetTangents(0);
		}
	}
	if ( Path.Num() > 0 )
		if ( GetActorLocation().Equals(Path.Last(), Tolerance) ){
			Destroy();
		}



	/*if ( Path.Num() > 0 ){
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
	}*/
}
