#pragma once



#include "FGAI_2/Astar/FAStar_Thread.h"
#include "FGAI_2/Astar/FGAStar.h"
#include "FGAI_2/Grid/FGGridActor.h"

#include "GameFramework/Pawn.h"

#include "ProfilingDebugging/ScopedTimers.h"

#include "FGPlayer.generated.h"

class FDurationTimer;
class AFGPathfindinder;
struct FFGTileinfo;
class UCameraComponent;
class AFGGridActor;
struct FAStar_Data;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFindPathAsyncDelegate);
UCLASS()
class AFGPlayer : public APawn, public IFGAStar {
	GENERATED_BODY()
public:
	AFGPlayer();

	virtual void BeginPlay() override;

	virtual void Tick( float DeltaSeconds ) override;

	virtual void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) override;

	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	AFGGridActor* CurrentGridActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	float HorizontalMovementSpeed = 3000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	float VerticalMovementSpeed = 5000.0f;
	//ASTAR STUFF BY VILHELM
	bool bMoveAlongPath;
	UPROPERTY()
	TArray<FVector> VectorPath;
	UPROPERTY()
	TArray<UFGNode*> CurrentPath;
	UPROPERTY()
	FVector FirstClickLoc;
	UPROPERTY()
	FVector SecondClickLoc;
	bool bIsSecondClick = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AFGPathfindinder> PathfinderClass;
	//AStarAsync
	FFindPathAsyncDelegate OnAsyncPathCompleteDelegate;
	FAStar_Data* AStar_Data;
	FAStar_Thread* AStar_Thread;
	double Time = 0;
	FDurationTimer DurationTimer = FDurationTimer(Time);


	/*
	* Callback for whenever the user presses "Confirm" aka the left mouse button
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Player", meta = (DisplayName = "OnConfirm"))
	void BP_OnConfirm( bool BP_bIsSecondClick );

	UFUNCTION(BlueprintPure, Category = "Player")
	bool GetMouseLocationOnGrid( FVector& OutWorldMouseLocation ) const;

private:
	void UpdateMovement( float DeltaTime );

	void Handle_Forward( float Value );
	void Handle_Right( float Value );
	void Handle_Up( float Value );
	void Handle_ConfirmedPressed();
	UFUNCTION()
	void HandleAsyncPathComplete();

	FVector InputVector;
};
