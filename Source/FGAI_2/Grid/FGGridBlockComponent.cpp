#include "FGGridBlockComponent.h"
#include "FGGridActor.h"

UFGGridBlockComponent::UFGGridBlockComponent()
{
	bWantsOnUpdateTransform = true;
}

FTransform UFGGridBlockComponent::GetBlockTransform() const
{
	if (GetOwner() == nullptr)
	{
		return FTransform::Identity;
	}

	FVector CustomRelativeLocation = GetRelativeLocation();
	CustomRelativeLocation.Z = 0.0f;

	const FVector TransformedRelativeLocation_ZeroZ = GetOwner()->GetActorTransform().TransformPositionNoScale(CustomRelativeLocation);

	return FTransform(GetOwner()->GetActorRotation(), TransformedRelativeLocation_ZeroZ);
}

void UFGGridBlockComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	AFGGridActor* GridOwner = Cast<AFGGridActor>(GetOwner());

	if (GridOwner == nullptr)
	{
		return;
	}

	GridOwner->UpdateBlockingTiles();
}

void UFGGridBlockComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	AFGGridActor* GridOwner = Cast<AFGGridActor>(GetOwner());

	if (GridOwner == nullptr)
	{
		return;
	}

	GridOwner->UpdateBlockingTiles();
}

