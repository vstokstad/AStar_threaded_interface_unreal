// Fill out your copyright notice in the Description page of Project Settings.


#include "FGPathFindingComponent.h"


// Sets default values for this component's properties
UFGPathFindingComponent::UFGPathFindingComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFGPathFindingComponent::BeginPlay(){
	Super::BeginPlay();

	// ...

}


void UFGPathFindingComponent::FindPathTo( FVector TargetLocation ){
	if ( Player == nullptr ){
		Player = GetOwner<AFGPlayer>();
	}
	if ( !CurrentGridActor->IsWorldLocationInsideGrid(TargetLocation) ){
		UE_LOG(LogTemp, Log, TEXT("TARGET IS OUTSIDE GRID"))
		return;
	}
	CurrentGridActor = Player->CurrentGridActor;

	AllTiles = CurrentGridActor->TileList;
	int tileIndex = CurrentGridActor->GetTileIndexFromWorldLocation(TargetLocation);
	if ( CurrentGridActor->IsTileIndexValid(tileIndex) ){
		TargetTile = CurrentGridActor->TileList[tileIndex];
	}
	FindPathTo(TargetTile);


}
void UFGPathFindingComponent::UpdateCurrentTile(){
	ensure(Player);
	CurrentGridActor = Player->CurrentGridActor;

	int tileIndex = CurrentGridActor->GetTileIndexFromWorldLocation(Player->GetActorLocation());
	if ( CurrentGridActor->IsTileIndexValid(tileIndex) ){
		CurrentTile = CurrentGridActor->TileList[tileIndex];
	}
}

void UFGPathFindingComponent::EvaluateTile( FFGTileinfo Tile ){
	if ( Tile.bBlock ){

	}
	else{
		Tile.H = FVector::Distance(Tile.RightTile->WorldLocation, TargetTile.WorldLocation);

	}
}
void UFGPathFindingComponent::GetNeighbours( FFGTileinfo Tile ){

}

void UFGPathFindingComponent::FindPathTo( FFGTileinfo Target ){
	if ( Player == nullptr ){
		Player = GetOwner<AFGPlayer>();
	}
	ensure(Player);
	CurrentGridActor = Player->CurrentGridActor;
	UpdateCurrentTile();
	TargetTile = Target;
	EvaluateTile(CurrentTile);
}

// Called every frame
void UFGPathFindingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
