#include "GenericGraphRuntimePrivatePCH.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraph::UGenericGraph()
{
	NodeFilter = UObject::StaticClass();

	EdGraph = nullptr;
}

UGenericGraph::~UGenericGraph()
{

}

#undef LOCTEXT_NAMESPACE
