
#pragma once

#include "CoreMinimal.h"

class FGenericGraphAssetEditor;
class FExtender;
class FToolBarBuilder;

class FGenericGraphAssetEditorToolbar : public TSharedFromThis<FGenericGraphAssetEditorToolbar>
{
public:
	FGenericGraphAssetEditorToolbar(TSharedPtr<FGenericGraphAssetEditor> InGenericGraphEditor)
		: GenericGraphEditor(InGenericGraphEditor) {}

	void AddGenericGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillGenericGraphToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FGenericGraphAssetEditor> GenericGraphEditor;
};
