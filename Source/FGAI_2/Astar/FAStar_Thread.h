// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGAStar.h"

#include "ProfilingDebugging/ScopedTimers.h"

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
	double Time = 0;
	FDurationTimer Timer = FDurationTimer(Time);
	bool bRunning = false;

	virtual bool Init() override{
		if ( Data != nullptr ){
			return true;
		}
		return false;
	}

	virtual uint32 Run() override{
		Timer.Start();
		bRunning = true;
		Data->Path = GetPathAsync(Data->GridActor, Data->Start, Data->End);
		return 0;
	}
	virtual void Exit() override{
		ensure(Data->Path.Num()>0);
		Timer.Stop();
		const float fTime = static_cast<float>(Time);
		UE_LOG(LogTemp, Log, TEXT("InsideAsyncTime:%f"), fTime)
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
