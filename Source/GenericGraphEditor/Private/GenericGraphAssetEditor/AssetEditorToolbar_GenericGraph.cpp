#include "GenericGraphAssetEditor/AssetEditorToolbar_GenericGraph.h"
#include "GenericGraphAssetEditor/AssetEditor_GenericGraph.h"
#include "GenericGraphAssetEditor/EditorCommands_GenericGraph.h"
#include "GenericGraphAssetEditor/GenericGraphEditorStyle.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_GenericGraph"

void FAssetEditorToolbar_GenericGraph::AddGenericGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(GenericGraphEditor.IsValid());
	TSharedPtr<FAssetEditor_GenericGraph> GenericGraphEditorPtr = GenericGraphEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GenericGraphEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP( this, &FAssetEditorToolbar_GenericGraph::FillGenericGraphToolbar ));
	GenericGraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_GenericGraph::FillGenericGraphToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(GenericGraphEditor.IsValid());
	TSharedPtr<FAssetEditor_GenericGraph> GenericGraphEditorPtr = GenericGraphEditor.Pin();

	ToolbarBuilder.BeginSection("Generic Graph");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_GenericGraph::Get().GraphSettings,
			NAME_None,
			LOCTEXT("GraphSettings_Label", "Graph Settings"),
			LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings"));
	}
	ToolbarBuilder.EndSection();

	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_GenericGraph::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FGenericGraphEditorStyle::GetStyleSetName(), "GenericGraphEditor.AutoArrange"));
	}
	ToolbarBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
