#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetEditorToolbar.h"
#include "GenericGraphAssetEditor.h"
#include "GenericGraphEditorCommands.h"


#define LOCTEXT_NAMESPACE "GenericGraphAssetEditorToolbar"

void FGenericGraphAssetEditorToolbar::AddGenericGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(GenericGraphEditor.IsValid());
	TSharedPtr<FGenericGraphAssetEditor> GenericGraphEditorPtr = GenericGraphEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GenericGraphEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP( this, &FGenericGraphAssetEditorToolbar::FillGenericGraphToolbar ));
	GenericGraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FGenericGraphAssetEditorToolbar::FillGenericGraphToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(GenericGraphEditor.IsValid());
	TSharedPtr<FGenericGraphAssetEditor> GenericGraphEditorPtr = GenericGraphEditor.Pin();

	ToolbarBuilder.BeginSection("Generic Graph");
	{

		const FText GraphSettingsLabel = LOCTEXT("GraphSettings_Label", "Graph Settings");
		const FText GraphSettingsTip = LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings");
		const FSlateIcon GraphSettingsIcon = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings");
		ToolbarBuilder.AddToolBarButton(FGenericGraphEditorCommands::Get().GraphSettings, 
			NAME_None,
			GraphSettingsLabel,
			GraphSettingsTip,
			GraphSettingsIcon);
	}
	ToolbarBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
