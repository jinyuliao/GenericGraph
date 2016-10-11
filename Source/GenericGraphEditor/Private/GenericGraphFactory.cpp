#include "GenericGraphEditorPrivatePCH.h"
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
// 	UDepGenericGraph* NewObjectAsset = ConstructObject(Class, InParent, Name, Flags | RF_Transactional);
// 	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE
