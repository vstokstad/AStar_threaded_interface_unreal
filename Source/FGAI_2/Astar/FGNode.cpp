// Fill out your copyright notice in the Description page of Project Settings.


#include "FGNode.h"

#include "FGAStar.h"

UFGNode::UFGNode(){

}
void UFGNode::SetSize( float Size ){
	NodeHalfSize = Size * 0.5f;
}
void UFGNode::InitNode( int _X, int _Y ){
	this->X = _X;
	this->Y = _Y;
}

float UFGNode::CalculateH( UFGNode* EndNode ){
	int x = this->X;
	int y = this->Y;
	int destX = EndNode->X;
	int destY = EndNode->Y;

	int H = (sqrt((x - destX) * (x - destX)
		+ (y - destY) * (y - destY)));
	this->HCost = H;
	//this->HCost=IAStar::GetDistance(this, EndNode);
	return HCost;
}
float UFGNode::CalculateG( UFGNode* StartNode ){
	int x = this->X;
	int y = this->Y;
	int StartX = StartNode->X;
	int StartY = StartNode->Y;

	int G = (sqrt((x - StartX) * (x - StartX)
		+ (y - StartY) * (y - StartY)));
	GCost = G;
	//this->GCost = IAStar::GetDistance(this, StartNode);
	return GCost;
}

