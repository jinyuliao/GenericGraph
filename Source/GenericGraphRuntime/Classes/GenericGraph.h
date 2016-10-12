#pragma once

#include "GenericGraphNode.h"
#include "GenericGraph.generated.h"

// test comment
UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraph : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraph();
	virtual ~UGenericGraph();

	UPROPERTY(EditAnywhere, Category = "GenericGraph")
	TSubclassOf<UGenericGraphNode> NodeType;

	UPROPERTY(EditAnywhere, Category = "GenericGraph")
	FString Name;
};
