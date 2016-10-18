#pragma once

#include "GenericGraphEdGraph.generated.h"

UCLASS()
class UGenericGraphEdGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UGenericGraphEdGraph();
	virtual ~UGenericGraphEdGraph();

	virtual void RebuildGenericGraph();

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif
};
