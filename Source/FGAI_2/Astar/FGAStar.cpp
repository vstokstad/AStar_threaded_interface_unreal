// Fill out your copyright notice in the Description page of Project Settings.


#include "FGAStar.h"

#include "Kismet/KismetSystemLibrary.h"


// Add default functionality here for any IAstar functions that are not pure virtual.
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

	UFGNode* CurrentNode = StartNode;

	Open.Add(CurrentNode);
	bool bSuccess = false;

	while ( Open.Num() > 0 ){
		if ( CurrentNode == EndNode ){
			bSuccess = true;
			break;
		}
		CurrentNode->GCost = GetDistance(CurrentNode, StartNode);
		CurrentNode->HCost = GetDistance(CurrentNode, EndNode);
		TArray<UFGNode*> Neighbours = GetNeighbours(CurrentNode, NodeGrid, Grid->Height, Grid->Width, Closed);
		for ( auto& Neighbour : Neighbours ){
			if ( Neighbour == nullptr || Neighbour->bIsBlocked || Closed.Contains(Neighbour) ){
				continue;
			}

			//int newMoveCost = CurrentNode->GCost + GetDistance(CurrentNode, StartNode);

			if ( !Open.Contains(Neighbour) ){
				Neighbour->GCost = GetDistance(Neighbour, StartNode);
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
			
			Algo::Sort(Open);
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
			float DebugTime = 5.f;
			//UKismetSystemLibrary::DrawDebugArrow(Grid, PathNode->WorldLocation, PathNode->Parent->WorldLocation, 5.f, FColor::Purple, DebugTime, 10.f);
			UKismetSystemLibrary::DrawDebugSphere(Grid, PathNode->WorldLocation, 100.f, 12, FColor::Purple, DebugTime, 10.f);

		}
		return Path;

	}
	//TODO DEBUG-REMOVE
	UE_LOG(LogTemp, Log, TEXT("COULD NOT FIND PATH"));
	return
	TArray<UFGNode*>();
}
TArray<UFGNode*> IFGAStar::RetracePath( UFGNode* StartNode, UFGNode* EndNode ){
	TArray<UFGNode*> Path;
	UFGNode* CurrentNode = EndNode;
	while ( CurrentNode != StartNode ){
		Path.Add(CurrentNode);
		ensure(CurrentNode->Parent!=nullptr);
		CurrentNode = CurrentNode->Parent;

	}
	Algo::Reverse(Path);
	return Path;
}
int IFGAStar::GetDistance( const UFGNode* A, const UFGNode* B ){
	// int distX = FMath::Abs(A->X - B->X);
	// int distY = FMath::Abs(A->Y - B->Y);
	// if ( distX > distY ){
	// 	return 14 * distY + 10 * (distX - distY);
	// }
	// return 14 * distX + 10 * (distY - distX);
	int x = A->WorldLocation.X;
	int y = A->WorldLocation.Y;
	int destX = B->WorldLocation.X;
	int destY = B->WorldLocation.Y;
	return sqrt((x - destX) * (x - destX)
		+ (y - destY) * (y - destY));
}
void IFGAStar::InitNodeGrid( AFGGridActor* Grid, TMap<FIntPoint, UFGNode*>& NodeGrid ){
	for ( int x = 0; x < Grid->Width; ++x ){
		for ( int y = 0; y < Grid->Height; ++y ){
			int index = Grid->GetTileIndexFromXY(x, y);
			UFGNode* Node = NewObject<UFGNode>();
			Node->InitNode(x, y);
			Node->SetSize(Grid->TileSize);
			Node->bIsBlocked = Grid->TileList[index].bBlock;
			Node->WorldLocation = Grid->GetWorldLocationFromXY(x, y);
			NodeGrid.Add(FIntPoint(x, y), Node);
			FString string = FString("X:");
			string.AppendInt(x);
			string.Append(" Y:").AppendInt(y);
			UKismetSystemLibrary::DrawDebugString(Grid, Node->WorldLocation, string, 0, FLinearColor::Black, 5.f);
		}
	}
}
TArray<UFGNode*> IFGAStar::GetNeighbours( UFGNode* Node, const TMap<FIntPoint, UFGNode*>& NodeGrid, int Height, int Width, const TArray<UFGNode*>& ClosedList ){
	TArray<UFGNode*> N;
	for ( int x = -1; x <= 1; ++x ){
		for ( int y = -1; y < 1; ++y ){
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
				N.Add(Neighbour);
			}
		}
	}
	Node->Neighbours = N;
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
