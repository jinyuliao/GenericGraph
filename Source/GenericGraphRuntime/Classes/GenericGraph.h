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

	UPROPERTY()
	TArray<UGenericGraphNode*> AllNodes;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;
#endif
};
