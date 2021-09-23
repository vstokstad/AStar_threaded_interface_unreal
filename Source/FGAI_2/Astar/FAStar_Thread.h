// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGAStar.h"

/**
 * 
 */

struct FAStar_Data {

	AFGGridActor* GridActor = nullptr;
	TArray<FVector> Path = {};
	FVector Start = FVector::ZeroVector;
	FVector End = FVector::ZeroVector;
};

struct FAStar_Thread : public FRunnable, public IFGAStar {
	friend IFGAStar;

	FAStar_Thread( FAStar_Data* InData ){
		Data = InData;
	}
	FAStar_Data* Data;

	bool bRunning = false;

	virtual bool Init() override{
		if ( Data != nullptr){
			return true;
		}
		return false;
	}

	virtual uint32 Run() override{
		bRunning = true;
		Data->Path = GetPathAsync(Data->GridActor, Data->Start, Data->End);
		if ( Data->Path.Num() > 0 ){
			Exit();
		}
		return 0;
	}
	virtual void Exit() override{
		ensure(Data->Path.Num()>0);
		bRunning = false;
	}
	virtual void Stop() override{
		bRunning = false;
	}
	void Destroy(){
		Data = nullptr;
		DestructItem(this);
	}
};
