#include "AssetTypeActions_GenericGraph.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphEdge.h"
#include "GenericGraphAssetEditor/SEdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/SEdNode_GenericGraphEdge.h"

DEFINE_LOG_CATEGORY(GenericGraphEditor)

#define LOCTEXT_NAMESPACE "Editor_GenericGraph"

class FGraphPanelNodeFactory_GenericGraph : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_GenericGraphNode* GraphEdNode = Cast<UEdNode_GenericGraphNode>(Node))
		{
			return SNew(SEdNode_GenericGraphNode, GraphEdNode);
		}
		else if (UEdNode_GenericGraphEdge* TransitionEdNode = Cast<UEdNode_GenericGraphEdge>(Node))
		{
			return SNew(SEdNode_GenericGraphEdge, TransitionEdNode);
		}
		return nullptr;
	}
};

TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_GenericGraph;

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
};

IMPLEMENT_MODULE( FGenericGraphEditor, UGenericGraphEditor )

void FGenericGraphEditor::StartupModule()
{
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
}

void FGenericGraphEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

