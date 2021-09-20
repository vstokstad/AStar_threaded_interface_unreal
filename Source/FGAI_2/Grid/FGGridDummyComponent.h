#pragma once

#include "Components/ActorComponent.h"
#include "FGGridDummyComponent.generated.h"

UCLASS()
class FGAI_2_API UFGGridDummyComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFGGridDummyComponent()
	{
		bIsEditorOnly = true;
	}
};
