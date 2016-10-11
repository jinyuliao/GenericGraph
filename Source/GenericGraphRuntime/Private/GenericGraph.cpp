#include "GenericGraphRuntimePrivatePCH.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
	NodeType = UGenericGraphNode::StaticClass();
}

UGenericGraph::~UGenericGraph()
{

}

#undef LOCTEXT_NAMESPACE
