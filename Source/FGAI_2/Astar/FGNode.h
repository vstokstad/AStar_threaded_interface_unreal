// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FGNode.generated.h"

/**
 * 
 */
UCLASS()
class FGAI_2_API UFGNode : public UObject {
	friend class IAStar;
	GENERATED_BODY()
public:
	UFGNode() = default;

	float NodeHalfSize = 250;
	int HCost;
	int GetHCost() const{
		return HCost;
	}
	int GCost;
	int GetGCost() const{
		return GCost;
	}

	int GetFCost() const{
		return GCost + HCost;
	}
	int X, Y;
	FVector WorldLocation;
	FColor Color;
	FColor GetColor(){
		Color = bIsBlocked ? FColor::Red : FColor::Emerald;
		return Color;
	}
	UPROPERTY()
	UFGNode* Parent;
	UPROPERTY()
	TArray<UFGNode*> Neighbours = {};

	bool bIsBlocked = false;

	void SetSize( float Size );
	void InitNode( int _X, int _Y );

	float CalculateH( UFGNode* EndNode );
	float CalculateG( UFGNode* StartNode );
};
inline bool operator <( const UFGNode& lhs, const UFGNode& rhs ){
	if ( lhs.GetFCost() == rhs.GetFCost() ){
		return lhs.GetHCost() < rhs.GetHCost();
	}
	return lhs.GetFCost() < rhs.GetFCost();
}
inline bool operator >( const UFGNode& lhs, const UFGNode& rhs ){
	if ( lhs.GetFCost() == rhs.GetFCost() ){
		return lhs.GetHCost() > rhs.GetHCost();
	}
	return lhs.GetFCost() > rhs.GetFCost();
}
inline bool operator ==( const UFGNode& lhs, const UFGNode& rhs ){
	return lhs.WorldLocation == rhs.WorldLocation;
}
