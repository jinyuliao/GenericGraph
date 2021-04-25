
#pragma once

#include "CoreMinimal.h"

class FAssetEditor_GenericGraph;
class FExtender;
class FToolBarBuilder;

class GENERICGRAPHEDITOR_API FAssetEditorToolbar_GenericGraph : public TSharedFromThis<FAssetEditorToolbar_GenericGraph>
{
public:
	FAssetEditorToolbar_GenericGraph(TSharedPtr<FAssetEditor_GenericGraph> InGenericGraphEditor)
		: GenericGraphEditor(InGenericGraphEditor) {}

	void AddGenericGraphToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillGenericGraphToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FAssetEditor_GenericGraph> GenericGraphEditor;
};
