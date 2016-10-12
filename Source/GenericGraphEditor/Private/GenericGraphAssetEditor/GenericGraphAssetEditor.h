#pragma once


class FGenericGraphAssetEditor : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
	FGenericGraphAssetEditor();
	virtual ~FGenericGraphAssetEditor();

	void InitGenericGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UGenericGraph* Graph);

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	// End of IToolkit interface

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	// End of FAssetEditorToolkit

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface

private:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

	void CreateInternalWidgets();
	TSharedRef<SGraphEditor> CreateViewportWidget();

	void ExtendToolbar();

	void BindCommands();

	void CreateEdGraph();

	//////////////////////////////////////////////////////////////////////////
	// graph editor event
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	void OnNodeDoubleClicked(UEdGraphNode* Node);

	/**
	* Called when a node's title is committed for a rename
	*
	* @param	NewText				New title text
	* @param	CommitInfo			How text was committed
	* @param	NodeBeingChanged	The node being changed
	*/
	void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);

private:
	UGenericGraph* EditingGraph;
	UEdGraph* EditingEdGraph;

	TSharedPtr<SGraphEditor> ViewportWidget;
	TSharedPtr<class IDetailsView> PropertyWidget;
};


