// Fill out your copyright notice in the Description page of Project Settings.


#include "FGAStar.h"

#include "Kismet/KismetSystemLibrary.h"



TArray<UFGNode*> IFGAStar::FindPath_Implementation( AFGGridActor* Grid, FVector Start, FVector End ){
	return GetPath(Grid, Start, End);
}
TArray<UFGNode*> IFGAStar::GetPath( AFGGridActor* Grid, FVector Start, FVector End ){
	TArray<UFGNode*> Open = {};
	TArray<UFGNode*> Closed = {};

	TMap<FIntPoint, UFGNode*> NodeGrid = {};
	InitNodeGrid(Grid, NodeGrid);

	UFGNode* StartNode = GetNodeFromWorldLoc(Start, NodeGrid);
	UFGNode* EndNode = GetNodeFromWorldLoc(End, NodeGrid);
	if ( StartNode == nullptr || EndNode == nullptr ){
		return TArray<UFGNode*>();
	}

	UFGNode* CurrentNode = StartNode;

	Open.Add(CurrentNode);
	bool bSuccess = false;
	CurrentNode->HCost = GetDistance(CurrentNode, EndNode);
	while ( Open.Num() > 0 ){
		if ( CurrentNode == EndNode ){
			bSuccess = true;
			break;
		}

		TArray<UFGNode*> Neighbours = GetNeighbours(CurrentNode, NodeGrid, Grid->Height, Grid->Width, Closed);
		UE_LOG(LogTemp, Log, TEXT("NumberOfNeighbours: %i"), Neighbours.Num())
		for ( auto& Neighbour : Neighbours ){
			if ( Neighbour == nullptr || Neighbour->bIsBlocked || Closed.Contains(Neighbour) ){
				continue;
			}
			int NewGCost = CurrentNode->GCost + GetDistance(CurrentNode, Neighbour);
			if ( !Open.Contains(Neighbour) ){

				Neighbour->GCost = NewGCost;
				Neighbour->HCost = GetDistance(Neighbour, EndNode);
				Open.Add(Neighbour);
			}
			else{
				Neighbour->Parent = CurrentNode;
			}
		}

		Open.RemoveSingle(CurrentNode);
		Closed.Add(CurrentNode);
		if ( Open.Num() > 0 ){
			Open.Sort();
			//Algo::Sort(Open);
			CurrentNode = Open[0];
		}
	}

	if
	( bSuccess ){

		TArray<UFGNode*> Path = RetracePath(StartNode, EndNode);
		ensure(Path.Num()>0);
		//TODO Remove Debug
		UE_LOG(LogTemp, Log, TEXT("CurrentNode is EndNode"))
		for ( auto& PathNode : Path ){
			float DebugTime = 2.f;
			UKismetSystemLibrary::DrawDebugArrow(Grid, PathNode->WorldLocation, PathNode->Parent->WorldLocation, 5.f, FColor::Purple, DebugTime, 10.f);
			UKismetSystemLibrary::DrawDebugSphere(Grid, PathNode->WorldLocation, 100.f, 12, FColor::Purple, DebugTime, 10.f);

		}
		Open.Reset();
		Closed.Reset();
		NodeGrid.Reset();
		return Path;

	}
	//TODO DEBUG-REMOVE
	UE_LOG(LogTemp, Log, TEXT("COULD NOT FIND PATH"));
	Open.Reset();
	Closed.Reset();
	NodeGrid.Reset();
	return TArray<UFGNode*>();
}
TArray<UFGNode*> IFGAStar::RetracePath( UFGNode* StartNode, UFGNode* EndNode ){
	TArray<UFGNode*> Path;
	UFGNode* CurrentNode = EndNode;
	while ( CurrentNode != StartNode ){
		Path.Add(CurrentNode);
		ensure(CurrentNode->Parent!=nullptr);
		CurrentNode = CurrentNode->Parent;

	}
	//	Algo::Reverse(Path);
	return Path;
}
int IFGAStar::GetDistance( const UFGNode* A, const UFGNode* B ){
	int distX = FMath::Abs(A->WorldLocation.X - B->WorldLocation.X);
	int distY = FMath::Abs(A->WorldLocation.Y - B->WorldLocation.Y);
	if ( distX > distY ){
		return FMath::Abs(14 * distY + 10 * (distX - distY));
	}
	return FMath::Abs(14 * distX + 10 * (distY - distX));

}
void IFGAStar::InitNodeGrid( AFGGridActor* Grid, TMap<FIntPoint, UFGNode*>& NodeGrid ){
	for ( int x = Grid->Width - 1; x >= 0; --x ){
		for ( int y = Grid->Height - 1; y >= 0; --y ){
			int index = Grid->GetTileIndexFromXY(x, y);
			UFGNode* Node = NewObject<UFGNode>();
			Node->InitNode(x, y);
			Node->SetSize(Grid->TileSize);
			Node->bIsBlocked = Grid->TileList[index].bBlock;
			Node->WorldLocation = Grid->GetWorldLocationFromXY(x, y);
			NodeGrid.Add(FIntPoint(x, y), Node);
			//TODO Remove Debug stuff;
			FString string = FString("X:");
			string.AppendInt(x);
			string.Append(" Y:").AppendInt(y);
			UKismetSystemLibrary::DrawDebugString(Grid, Node->WorldLocation, string, 0, FLinearColor::Black, 1.f);
		}
	}
}
TArray<UFGNode*> IFGAStar::GetNeighbours( UFGNode* Node, const TMap<FIntPoint, UFGNode*>& NodeGrid, int Height, int Width, const TArray<UFGNode*>& ClosedList ){
	for ( int x = -1; x <= 1; x++ ){
		for ( int y = -1; y < 1; y++ ){
			if ( x == 0 && y == 0 ){
				continue;
			}
			int checkX = Node->X + x;
			int checkY = Node->Y + y;
			if ( checkX >= 0 && checkX < Width && checkY >= 0 && checkY < Height ){
				UFGNode* Neighbour = NodeGrid.FindRef(FIntPoint(checkX, checkY));
				if ( ClosedList.Contains(Neighbour) )
					continue;
				Neighbour->Parent = Node;
				Node->Neighbours.Add(Neighbour);
			}
		}
	}
	return Node->Neighbours;
}
UFGNode* IFGAStar::GetNodeFromWorldLoc( const FVector& Location, const TMap<FIntPoint, UFGNode*>& NodeGrid ){
	for ( auto Node : NodeGrid ){

		if ( Node.Value->WorldLocation.Equals(Location, Node.Value->NodeHalfSize) ){
			return Node.Value;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("LOCATION HAS NO MATCHIN NODE"));
	return nullptr;
}
