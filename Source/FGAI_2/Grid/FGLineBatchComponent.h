// Copyright Epic Games, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "DynamicMeshBuilder.h"
#include "FGLineBatchComponent.generated.h"

class FPrimitiveSceneProxy;
class FMaterialRenderProxy;
class FColoredMaterialRenderProxy;

USTRUCT()
struct FFGBatchedLine
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector Start;

	UPROPERTY()
	FVector End;

	UPROPERTY()
	FLinearColor Color;

	UPROPERTY()
	float Thickness;

	UPROPERTY()
	float RemainingLifeTime;

	UPROPERTY()
	uint8 DepthPriority;

	FFGBatchedLine()
		: Start(ForceInit)
		, End(ForceInit)
		, Color(ForceInit)
		, Thickness(0)
		, RemainingLifeTime(0)
		, DepthPriority(0)
	{}
	FFGBatchedLine(const FVector& InStart, const FVector& InEnd, const FLinearColor& InColor, float InLifeTime, float InThickness, uint8 InDepthPriority)
		:	Start(InStart)
		,	End(InEnd)
		,	Color(InColor)
		,	Thickness(InThickness)
		,	RemainingLifeTime(InLifeTime)
		,	DepthPriority(InDepthPriority)
	{}
};

USTRUCT()
struct FFGBatchedPoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	FLinearColor Color;

	UPROPERTY()
	float PointSize;

	UPROPERTY()
	float RemainingLifeTime;

	UPROPERTY()
	uint8 DepthPriority;

	FFGBatchedPoint()
		: Position(ForceInit)
		, Color(ForceInit)
		, PointSize(0)
		, RemainingLifeTime(0)
		, DepthPriority(0)
	{}
	FFGBatchedPoint(const FVector& InPosition, const FLinearColor& InColor, float InPointSize, float InLifeTime, uint8 InDepthPriority)
		:	Position(InPosition)
		,	Color(InColor)
		,	PointSize(InPointSize)
		,	RemainingLifeTime(InLifeTime)
		,	DepthPriority(InDepthPriority)
	{}
};

struct FFGBatchedMesh
{
public:
	FFGBatchedMesh()
		: RemainingLifeTime(0)
	{};

	/**
	 * MeshVerts - linear array of world space vertex positions
	 * MeshIndices - array of indices into MeshVerts.  Each triplet is a tri.  i.e. [0,1,2] is first tri, [3,4,5] is 2nd tri, etc
	 */
	FFGBatchedMesh(TArray<FVector> const& InMeshVerts, TArray<int32> const& InMeshIndices, FColor const& InColor, uint8 InDepthPriority, float LifeTime)
		: MeshVerts(InMeshVerts), MeshIndices(InMeshIndices), 
		  Color(InColor), DepthPriority(InDepthPriority), RemainingLifeTime(LifeTime)
	{}

	TArray<FVector> MeshVerts;
	TArray<int32> MeshIndices;
	FColor Color;
	uint8 DepthPriority;
	float RemainingLifeTime;
};

/** 
 * The line batch component buffers and draws lines (and some other line-based shapes) in a scene. 
 *	This can be useful for debug drawing, but is not very performant for runtime use.
 */
UCLASS(MinimalAPI)
class UFGLineBatchComponent : public UPrimitiveComponent
{
	GENERATED_UCLASS_BODY()

	/** Buffer of lines to draw */
	TArray<struct FFGBatchedLine> BatchedLines;
	/** Buffer or points to draw */
	TArray<struct FFGBatchedPoint> BatchedPoints;
	/** Default time that lines/points will draw for */
	float DefaultLifeTime;
	/** Buffer of simple meshes to draw */
	TArray<struct FFGBatchedMesh> BatchedMeshes;
	/** Whether to calculate a tight accurate bounds (encompassing all points), or use a giant bounds that is fast to compute. */
	uint32 bCalculateAccurateBounds:1;

	/** Provide many lines to draw - faster than calling DrawLine many times. */
	void DrawLines(const TArray<FFGBatchedLine>& InLines);

	/** Draw a box */
	void DrawBox(const FBox& Box, const FMatrix& TM, const FColor& Color, uint8 InDepthPriorityGroup);

	/** Draw an arrow */
	void DrawDirectionalArrow(const FMatrix& ArrowToWorld,FColor InColor,float Length,float ArrowSize,uint8 DepthPriority);

	/** Draw a circle */
	void DrawCircle(const FVector& Base, const FVector& X, const FVector& Y, FColor Color, float Radius, int32 NumSides, uint8 DepthPriority);

	virtual void DrawLine(
		const FVector& Start,
		const FVector& End,
		const FLinearColor& Color,
		uint8 DepthPriority,
		float Thickness = 0.0f,
		float LifeTime = 0.0f
		);
	virtual void DrawPoint(
		const FVector& Position,
		const FLinearColor& Color,
		float PointSize,
		uint8 DepthPriority,
		float LifeTime = 0.0f
		);

	/** Draw a box */
	void DrawSolidBox(FBox const& Box, FTransform const& Xform, const FColor& Color, uint8 DepthPriority, float LifeTime);
	/** Draw a mesh */
	void DrawMesh(TArray<FVector> const& Verts, TArray<int32> const& Indices, FColor const& Color, uint8 DepthPriority, float LifeTime);

	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ End UPrimitiveComponent Interface.
	
	
	//~ Begin UActorComponent Interface.
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void ApplyWorldOffset(const FVector& InOffset, bool bWorldShift) override;
	//~ End UActorComponent Interface.

	/** Clear all batched lines, points and meshes */
	void Flush();

	static TMap<FColor, FColoredMaterialRenderProxy*> GlobalColorMap;

	static FColoredMaterialRenderProxy* GetOrCreateColoredMaterial(const FColor& InColor);
};




/** Represents a LineBatchComponent to the scene manager. */
class FFGLineBatcherSceneProxy : public FPrimitiveSceneProxy
{
public:
	SIZE_T GetTypeHash() const override;

	FFGLineBatcherSceneProxy(const UFGLineBatchComponent* InComponent);

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;

	/**
	*  Returns a struct that describes to the renderer when to draw this proxy.
	*	@param		Scene view to use to determine our relevence.
	*  @return		View relevance struct
	*/
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
	virtual uint32 GetMemoryFootprint( void ) const override;
	uint32 GetAllocatedSize( void ) const;

private:
	TArray<FFGBatchedLine> Lines;
	TArray<FFGBatchedPoint> Points;
	TArray<FFGBatchedMesh> Meshes;
};
