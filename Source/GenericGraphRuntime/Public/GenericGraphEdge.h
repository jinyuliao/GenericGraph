#pragma once

#include "CoreMinimal.h"
#include "GenericGraphNode.h"
#include "GenericGraphEdge.generated.h"

class UGenericGraph;

UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphEdge : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphEdge();
	virtual ~UGenericGraphEdge();

	UPROPERTY(VisibleAnywhere, Category = "GenericGraphNode")
	UGenericGraph* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphEdge")
	UGenericGraphNode* StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphEdge")
	UGenericGraphNode* EndNode;

	UFUNCTION(BlueprintPure, Category = "GenericGraphEdge")
	UGenericGraph* GetGraph() const;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	bool bShouldDrawTitle = false;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FText NodeTitle;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphEdge")
	FLinearColor EdgeColour = FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);
#endif

#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return NodeTitle; }
	FLinearColor GetEdgeColour() { return EdgeColour; }

	virtual void SetNodeTitle(const FText& NewTitle);
#endif
	
};
