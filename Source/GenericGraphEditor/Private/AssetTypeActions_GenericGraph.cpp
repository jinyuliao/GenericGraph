#include "AssetTypeActions_GenericGraph.h"
#include "GenericGraphEditorPCH.h"
#include "GenericGraphAssetEditor/AssetEditor_GenericGraph.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_GenericGraph"

FAssetTypeActions_GenericGraph::FAssetTypeActions_GenericGraph(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_GenericGraph::GetName() const
{
	return LOCTEXT("FGenericGraphAssetTypeActionsName", "Generic Graph");
}

FColor FAssetTypeActions_GenericGraph::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_GenericGraph::GetSupportedClass() const
{
	return UGenericGraph::StaticClass();
}

void FAssetTypeActions_GenericGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UGenericGraph* Graph = Cast<UGenericGraph>(*ObjIt))
		{
			TSharedRef<FAssetEditor_GenericGraph> NewGraphEditor(new FAssetEditor_GenericGraph());
			NewGraphEditor->InitGenericGraphAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_GenericGraph::GetCategories()
{
	return MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE