#include "AssetEditorToolbar_GenericGraph.h"
#include "AssetEditor_GenericGraph.h"
#include "EditorCommands_GenericGraph.h"

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

		const FText GraphSettingsLabel = LOCTEXT("GraphSettings_Label", "Graph Settings");
		const FText GraphSettingsTip = LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings");
		const FSlateIcon GraphSettingsIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings");
		ToolbarBuilder.AddToolBarButton(FEditorCommands_GenericGraph::Get().GraphSettings,
			NAME_None,
			GraphSettingsLabel,
			GraphSettingsTip,
			GraphSettingsIcon);
	}
	ToolbarBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
