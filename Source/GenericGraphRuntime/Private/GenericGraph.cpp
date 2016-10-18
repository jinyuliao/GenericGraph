#include "GenericGraphRuntimePrivatePCH.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
#endif
}

UGenericGraph::~UGenericGraph()
{

}

void UGenericGraph::ClearGraph()
{
	for (int i = 0; i < AllNodes; ++i)
	{
		UGenericGraphNode* Node = AllNodes[i];

		Node->ParentNodes.Reset();
		Node->ChildrenNodes.Reset();
	}

	AllNodes.Reset();
	RootNodes.Reset();
}

#undef LOCTEXT_NAMESPACE
