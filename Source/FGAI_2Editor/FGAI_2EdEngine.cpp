#include "FGAI_2EdEngine.h"
#include "FGGridBlockVisualizer.h"
#include "FGAI_2/Grid/FGGridBlockComponent.h"

void UFGAI_2EdEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);

	GridBlockVisualizer = MakeShareable(new FFGGridBlockVisualizer);

	RegisterComponentVisualizer(UFGGridBlockComponent::StaticClass()->GetFName(), GridBlockVisualizer);
}
