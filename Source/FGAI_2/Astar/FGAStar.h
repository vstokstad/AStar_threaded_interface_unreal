// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGNode.h"

#include "FGAI_2/Grid/FGGridActor.h"

#include "UObject/Interface.h"

#include "FGAStar.generated.h"


struct FAStar_Data;
struct FAStar_Thread;

UINTERFACE()
class UFGAStar : public UInterface {
	GENERATED_BODY()

};

/**
 * 
 */

class FGAI_2_API IFGAStar {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<UFGNode*> FindPath( AFGGridActor* Grid, FVector Start, FVector End );
	TArray<UFGNode*> FindPath_Implementation( AFGGridActor* Grid, FVector Start, FVector End );

	void OnAsyncPathComplete();

	void FindPathAsync( FAStar_Thread* Thread );

	static int GetDistance( const UFGNode* A, const UFGNode* B );
	TArray<FVector> SmoothPath( TArray<UFGNode*> NodePath );
private:
	TArray<UFGNode*> GetPath( AFGGridActor* Grid, FVector Start, FVector End );
	TArray<UFGNode*> RetracePath( UFGNode* StartNode, UFGNode* EndNode );
	void InitNodeGrid( AFGGridActor* Grid, TMap<FIntPoint, UFGNode*>& NodeGrid );
	TArray<UFGNode*> GetNeighbours( UFGNode* Node, const TMap<FIntPoint, UFGNode*>& NodeGrid, int Height, int Width, const TArray<UFGNode*>& ClosedList );
	UFGNode* GetNodeFromWorldLoc( const FVector& Location, const TMap<FIntPoint, UFGNode*>& NodeGrid );
protected:
	TArray<FVector> GetPathAsync( AFGGridActor* Grid, FVector Start, FVector End );
};
