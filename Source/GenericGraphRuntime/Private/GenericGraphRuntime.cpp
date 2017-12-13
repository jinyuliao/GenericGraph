#include "GenericGraphRuntimePCH.h"

DEFINE_LOG_CATEGORY(GenericGraphRuntime)

class FGenericGraphRuntime : public IGenericGraphRuntime
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FGenericGraphRuntime, GenericGraphRuntime )



void FGenericGraphRuntime::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FGenericGraphRuntime::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



