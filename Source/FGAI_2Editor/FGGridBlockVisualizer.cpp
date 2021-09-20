#include "FGGridBlockVisualizer.h"
#include "FGAI_2/Grid/FGGridBlockComponent.h"
#include "DrawDebugHelpers.h"

void FFGGridBlockVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UFGGridBlockComponent* GridBlockComponent = Cast<UFGGridBlockComponent>(Component);

	if (GridBlockComponent != nullptr && GridBlockComponent->GetOwner() != nullptr)
	{
		const FMatrix Matrix = GridBlockComponent->GetBlockTransform().ToMatrixNoScale();
		const FVector ExtentsHalf = GridBlockComponent->Extents * 0.5f;
		const FBox Box(-ExtentsHalf, ExtentsHalf);
		::DrawWireBox(PDI, Matrix, Box, FLinearColor::Green, 0);
	}
}
