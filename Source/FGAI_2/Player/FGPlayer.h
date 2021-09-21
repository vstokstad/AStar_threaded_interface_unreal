#pragma once



#include "FGAI_2/Astar/FGAStar.h"
#include "FGAI_2/Grid/FGGridActor.h"

#include "GameFramework/Pawn.h"
#include "FGPlayer.generated.h"

struct FFGTileinfo;
class UCameraComponent;
class AFGGridActor;

UCLASS()
class AFGPlayer : public APawn, public IFGAStar {
	GENERATED_BODY()
public:
	AFGPlayer();

	virtual void BeginPlay() override;

	virtual void Tick( float DeltaSeconds ) override;

	virtual void SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ) override;


	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	UCameraComponent* CameraComponent = nullptr;


	UPROPERTY(BlueprintReadWrite, Category = "Player")
	AFGGridActor* CurrentGridActor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	float HorizontalMovementSpeed = 3000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	float VerticalMovementSpeed = 1000.0f;

	bool bMoveAlongPath;

	UPROPERTY()
	TArray<UFGNode*> CurrentPath;
	UPROPERTY()
	FVector FirstClickLoc;
	UPROPERTY()
	FVector SecondClickLoc;
	bool bIsSecondClick = false;
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

	FVector InputVector;
};
