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
	CurrentNode->Parent = StartNode;

	Open.Add(CurrentNode);
	bool bSuccess = false;
	CurrentNode->HCost = GetDistance(CurrentNode, EndNode);
	CurrentNode->GCost = 0;
	while ( Open.Num() > 0 ){
		if ( CurrentNode == EndNode ){
			bSuccess = true;
			break;
		}

		TArray<UFGNode*> Neighbours = GetNeighbours(CurrentNode, NodeGrid, Grid->Height, Grid->Width, Closed);
		UE_LOG(LogTemp, Log, TEXT("NumberOfNeighbours: %i"), Neighbours.Num())
		for ( auto& Neighbour : Neighbours ){
			if ( Neighbour == nullptr || Closed.Contains(Neighbour) || Neighbour->bIsBlocked ){
				continue;
			}

			int NewGCost = CurrentNode->GCost + GetDistance(CurrentNode, Neighbour);

			if ( !Open.Contains(Neighbour) || NewGCost < Neighbour->GCost ){

				Neighbour->GCost = NewGCost;
				Neighbour->HCost = GetDistance(Neighbour, EndNode);
				Neighbour->Parent = CurrentNode;
				Open.Add(Neighbour);
			}
			else{

				Neighbour->Parent = CurrentNode;


			}
		}

		Open.RemoveSingle(CurrentNode);
		Closed.Add(CurrentNode);
		if ( Open.Num() > 0 ){
			for ( int i = 0; i < Open.Num() - 1; ++i ){
				if ( Open[i]->GetFCost() < Open[i + 1]->GetFCost() ){
				}
				else{
					UFGNode* temp = Open[i];
					Open[i] = Open[i + 1];
					Open[i + 1] = temp;
				}
			}
			//	Open.Sort();
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
			float DebugTime = 1.f;
			UKismetSystemLibrary::DrawDebugArrow(Grid, PathNode->WorldLocation, PathNode->Parent->WorldLocation, 5.f, FColor::Purple, DebugTime, 5.f);
			UKismetSystemLibrary::DrawDebugSphere(Grid, PathNode->WorldLocation, 50.f, 8, FColor::Purple, DebugTime, 4.f);
		}

		return Path;

	}
	//TODO DEBUG-REMOVE
	float DebugTime = 1.f;
	for ( auto& Node : Open ){
		UKismetSystemLibrary::DrawDebugSphere(Grid, Node->WorldLocation, 50.f, 8, FColor::Green, DebugTime, 4.f);
	}
	for ( auto& Node : Closed ){
		UKismetSystemLibrary::DrawDebugSphere(Grid, Node->WorldLocation, 50.f, 8, FColor::Orange, DebugTime, 4.f);

	}
	UE_LOG(LogTemp, Log, TEXT("COULD NOT FIND PATH"));
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
	Algo::Reverse(Path);
	return Path;
}
int IFGAStar::GetDistance( const UFGNode* A, const UFGNode* B ){
	int distX = FMath::Abs(A->X - B->X);
	int distY = FMath::Abs(A->Y - B->Y);
	if ( distX > distY ){
		return (20 * distY + 10 * (distX - distY));
	}
	return (20 * distX + 10 * (distY - distX));

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
	FIntPoint Directions[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};
	FIntPoint Current = {Node->X, Node->Y};
	FIntPoint N = Current;
	bool Found[8];
	for ( int i = 0; i < 8; i++ ){
		N = Current + Directions[i];
		if ( N.X > (Width - 1) || N.X < 0 || N.Y > (Height - 1) || N.Y < 0 ){
			continue;
		}

		UFGNode* Neighbour = NodeGrid.FindChecked(N);
		if ( ClosedList.Contains(Neighbour) || Neighbour == nullptr || Neighbour->bIsBlocked )
			continue;
		Node->Neighbours.Add(Neighbour);
		Found[i] = true;
	}
	// for ( int i = 0; i < 4; i++ ){
	// 	int j = (i + 1) % 4;
	//
	// 	N = Current + Directions[i] + Directions[j];
	// 	UFGNode* Neighbour = NodeGrid.FindRef(N);
	//
	// 	if ( ClosedList.Contains(Neighbour) || Neighbour == nullptr )
	// 		continue;
	// 	Neighbour->Parent = Node;
	// 	Node->Neighbours.Add(Neighbour);
	// }

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
