// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "FGAI_2/Grid/FGGridActor.h"
#include "FGAI_2/Player/FGPlayer.h"

#include "FGPathFindingComponent.generated.h"



class AFGGridActor;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FGAI_2_API UFGPathFindingComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFGPathFindingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	AFGPlayer* Player;
	UPROPERTY(VisibleAnywhere, Category="ASTAR")
	AFGGridActor* CurrentGridActor;
	UPROPERTY(VisibleAnywhere, Category="ASTAR")
	FFGTileinfo CurrentTile;
	UPROPERTY()
	FFGTileinfo TargetTile;

	TArray<FFGTileinfo> AllTiles;
	TArray<FFGTileinfo> CalculatedTiles;
	UFUNCTION()
	void FindPathTo( FFGTileinfo Target );
	void FindPathTo( FVector TargetLocation );

	void UpdateCurrentTile();
	void EvaluateTile( FFGTileinfo Tile );
	void GetNeighbours( FFGTileinfo Tile );

	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;



};
