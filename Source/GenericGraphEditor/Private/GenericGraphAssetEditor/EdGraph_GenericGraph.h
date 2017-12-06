#pragma once

#include "CoreMinimal.h"
#include "EdGraph_GenericGraph.generated.h"

UCLASS()
class UEdGraph_GenericGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdGraph_GenericGraph();
	virtual ~UEdGraph_GenericGraph();

	virtual void RebuildGenericGraph();

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif
};
