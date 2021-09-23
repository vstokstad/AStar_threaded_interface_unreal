#include "FGPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "FGAI_2/Grid/FGGridActor.h"
#include "FGAI_2/Grid/FGPathfindinder.h"

#include "Kismet/GameplayStatics.h"

AFGPlayer::AFGPlayer(){
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
	//	PathFinderComp = CreateDefaultSubobject<UFGPathFindingComponent>(TEXT("PathFindingComp"));
}

void AFGPlayer::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> AllGridActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGGridActor::StaticClass(), AllGridActors);

	if ( AllGridActors.Num() > 0 ){
		// just pick the first for now
		CurrentGridActor = Cast<AFGGridActor>(AllGridActors[0]);
	}
	OnAsyncPathCompleteDelegate.AddDynamic(this, &AFGPlayer::HandleAsyncPathComplete);
	Time = 0;
}

void AFGPlayer::Tick( float DeltaSeconds ){
	Super::Tick(DeltaSeconds);

	if ( AStar_Thread != nullptr && !AStar_Thread->bRunning ){
		HandleAsyncPathComplete();
	}

	UpdateMovement(DeltaSeconds);


}

void AFGPlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent ){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFGPlayer::Handle_Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AFGPlayer::Handle_Right);
	PlayerInputComponent->BindAxis(TEXT("Up"), this, &AFGPlayer::Handle_Up);

	PlayerInputComponent->BindAction(TEXT("Confirm"), IE_Pressed, this, &AFGPlayer::Handle_ConfirmedPressed);
}
void AFGPlayer::EndPlay( const EEndPlayReason::Type EndPlayReason ){
	Super::EndPlay(EndPlayReason);
	OnAsyncPathCompleteDelegate.RemoveDynamic(this, &AFGPlayer::HandleAsyncPathComplete);
}


bool AFGPlayer::GetMouseLocationOnGrid( FVector& OutWorldMouseLocation ) const{
	if ( CurrentGridActor == nullptr )
		return false;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if ( PlayerController == nullptr )
		return false;

	float MouseX = 0.0f;
	float MouseY = 0.0f;

	PlayerController->GetMousePosition(MouseX, MouseY);

	FVector MouseWorldLocation;
	FVector MouseDirection;
	const FVector PlaneUp = CurrentGridActor->GetActorUpVector();
	const FVector PlaneLocation = CurrentGridActor->GetActorLocation();

	PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, MouseWorldLocation, MouseDirection);

	if ( MouseDirection.IsNearlyZero() )
		return false;

	float D = FVector::DotProduct(-PlaneUp, PlaneLocation);
	float T = -(D + FVector::DotProduct(PlaneUp, MouseWorldLocation)) / FVector::DotProduct(PlaneUp, MouseDirection);
	OutWorldMouseLocation = MouseWorldLocation + MouseDirection * T;

	return true;
}

void AFGPlayer::UpdateMovement( float DeltaTime ){
	if ( !InputVector.IsNearlyZero() ){
		FVector HorizontalMovement = GetActorTransform().TransformVectorNoScale(FVector(InputVector.X, InputVector.Y, 0.0f).GetSafeNormal());
		FVector VerticalMovement = FVector(0, 0, InputVector.Z);

		AddActorWorldOffset(HorizontalMovement * HorizontalMovementSpeed * DeltaTime);
		AddActorWorldOffset(VerticalMovement * VerticalMovementSpeed * DeltaTime);
	}

	InputVector = FVector::ZeroVector;
}

void AFGPlayer::Handle_Forward( float Value ){
	InputVector.X += Value;
}

void AFGPlayer::Handle_Right( float Value ){
	InputVector.Y += Value;
}

void AFGPlayer::Handle_Up( float Value ){
	InputVector.Z += Value;
}

void AFGPlayer::Handle_ConfirmedPressed(){
	FVector Loc;
	if ( GetMouseLocationOnGrid(Loc) ){

		if ( !bIsSecondClick ){
			BP_OnConfirm(false);
			FirstClickLoc = Loc;
			bIsSecondClick = true;
		}
		else{
			SecondClickLoc = Loc;
			BP_OnConfirm(true);
			DurationTimer.Start();
			CurrentPath = Execute_FindPath(this, CurrentGridActor, FirstClickLoc, SecondClickLoc);
			DurationTimer.Stop();
			float fTime = (Time);
			UE_LOG(LogTemp, Log, TEXT("SYNC COMPLETE IN: %f Seconds"), fTime)
			AStar_Data = new FAStar_Data{CurrentGridActor, VectorPath, FirstClickLoc, SecondClickLoc};
			AStar_Thread = new FAStar_Thread{AStar_Data};
			AStar_Thread->bRunning = true;
			DurationTimer.Start();
			FindPathAsync(AStar_Thread);
			bIsSecondClick = false;
		}

	};

}
void AFGPlayer::HandleAsyncPathComplete(){
	VectorPath.Empty(AStar_Thread->Data->Path.Num());
	VectorPath.Append(AStar_Thread->Data->Path);
	FRotator Direction = (SecondClickLoc - FirstClickLoc).Rotation();
	if ( PathfinderClass != nullptr ){
		GetWorld()->SpawnActor<AFGPathfindinder>(PathfinderClass, FirstClickLoc, Direction)->Path = VectorPath;
	}
	else{
		GetWorld()->SpawnActor<AFGPathfindinder>(FirstClickLoc, Direction)->Path = VectorPath;
	}
	DurationTimer.Stop();
	float fTime = (Time);
	UE_LOG(LogTemp, Log, TEXT("ASYNC COMPLETE IN: %f Seconds"), fTime)
	Time = 0.f;
	AStar_Thread->Destroy();
	AStar_Thread = nullptr;
	AStar_Data = nullptr;
}
