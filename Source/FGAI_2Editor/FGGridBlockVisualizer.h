#pragma once

#include "ComponentVisualizer.h"

class UActorComponent;

class FFGGridBlockVisualizer : public FComponentVisualizer
{
	/** Draw visualization for the supplied component */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
