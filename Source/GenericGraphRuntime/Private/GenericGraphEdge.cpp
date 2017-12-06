#include "GenericGraphEdge.h"

UGenericGraphEdge::UGenericGraphEdge()
{

}

UGenericGraphEdge::~UGenericGraphEdge()
{

}

UGenericGraph* UGenericGraphEdge::GetGraph() const
{
	return Cast<UGenericGraph>(GetOuter());
}
