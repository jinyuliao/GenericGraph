#include "GenericGraphEdge.h"

UGenericGraphEdge::UGenericGraphEdge()
{

}

UGenericGraphEdge::~UGenericGraphEdge()
{

}

UGenericGraph* UGenericGraphEdge::GetGraph() const
{
	return Graph;
}

#if WITH_EDITOR
void UGenericGraphEdge::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}
#endif // #if WITH_EDITOR