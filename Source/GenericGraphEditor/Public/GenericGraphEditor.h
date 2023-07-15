#pragma once
#include "Modules/ModuleManager.h"
#include "GenericGraphEditorModule.h"
#include <IAssetTools.h>
#include <EdGraphUtilities.h>

class FGenericGraphEditor : public IGenericGraphEditor
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type GenericGraphAssetCategoryBit;

	TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_GenericGraph;
};