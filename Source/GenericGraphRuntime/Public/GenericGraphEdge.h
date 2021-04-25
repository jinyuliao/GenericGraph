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

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphEdge")
	FLinearColor EdgeColour = FLinearColor(0.9f, 0.9f, 0.9f, 1.0f);

	UFUNCTION(BlueprintPure, Category = "GenericGraphEdge")
	UGenericGraph* GetGraph() const;

	UFUNCTION(BlueprintNativeEvent, Category = "GenericGraphEdge")
	FLinearColor GetEdgeColour() const;
	virtual FLinearColor GetEdgeColour_Implementation() const { return EdgeColour; }

};
