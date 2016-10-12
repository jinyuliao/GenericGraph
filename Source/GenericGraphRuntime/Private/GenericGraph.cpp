#include "GenericGraphRuntimePrivatePCH.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
	NodeType = UGenericGraphNode::StaticClass();

	EdGraph = nullptr;
}

UGenericGraph::~UGenericGraph()
{

}

#undef LOCTEXT_NAMESPACE
