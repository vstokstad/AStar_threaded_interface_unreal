#pragma once

#include "Components/ActorComponent.h"
#include "FGGridBlockComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class FGAI_2_API UFGGridBlockComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UFGGridBlockComponent();

	UFUNCTION(BlueprintPure, Category = "Grid")
	FTransform GetBlockTransform() const;

	UPROPERTY(EditAnywhere)
	FVector Extents = FVector(500.0f, 500.0f, 100.0f);

#if WITH_EDITOR
	/*
	* This is called whenever a property, on this component, is edited in the editor. Only available in the editor.
	* Only available in the editor. If you forget WITH_EDITOR you will get a compile error when compiling the non-editor build
	*/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

protected:
	/*
	* If bWantsOnUpdateTransform is set to true, this will be called whenever the component transform is modified.
	*/
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;
};
