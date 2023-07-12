#include "GenericGraphEditor.h"
#include "GenericGraphNodeFactory.h"
#include "AssetTypeActions_GenericGraph.h"
#include "GenericGraphAssetEditor/GenericGraphEditorStyle.h"

DEFINE_LOG_CATEGORY(GenericGraphEditor)

#define LOCTEXT_NAMESPACE "Editor_GenericGraph"

void FGenericGraphEditor::StartupModule()
{
	FGenericGraphEditorStyle::Initialize();

	GraphPanelNodeFactory_GenericGraph = MakeShareable(new FGraphPanelNodeFactory_GenericGraph());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_GenericGraph);

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	GenericGraphAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("GenericGraph")), LOCTEXT("GenericGraphAssetCategory", "GenericGraph"));

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_GenericGraph(GenericGraphAssetCategoryBit)));
}


void FGenericGraphEditor::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}

	if (GraphPanelNodeFactory_GenericGraph.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_GenericGraph);
		GraphPanelNodeFactory_GenericGraph.Reset();
	}

	FGenericGraphEditorStyle::Shutdown();
}

void FGenericGraphEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

IMPLEMENT_MODULE(FGenericGraphEditor, GenericGraphEditor)

#undef LOCTEXT_NAMESPACE

