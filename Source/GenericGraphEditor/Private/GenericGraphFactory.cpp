#include "GenericGraphFactory.h"

#define LOCTEXT_NAMESPACE "GenericGraph"

UGenericGraphFactory::UGenericGraphFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UGenericGraph::StaticClass();
}

UGenericGraphFactory::~UGenericGraphFactory()
{

}

UObject* UGenericGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE
