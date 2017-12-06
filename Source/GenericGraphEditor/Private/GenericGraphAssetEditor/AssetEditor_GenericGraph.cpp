#include "AssetEditor_GenericGraph.h"
#include "GenericGraphEditorPrivatePCH.h"
#include "AssetEditorToolbar_GenericGraph.h"
#include "AssetGraphSchema_GenericGraph.h"
#include "EditorCommands_GenericGraph.h"
#include "EdGraph_GenericGraph.h"
#include "AssetToolsModule.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "GenericCommands.h"
#include "GraphEditorActions.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "EdGraphUtilities.h"

#define LOCTEXT_NAMESPACE "AssetEditor_GenericGraph"

const FName GenericGraphEditorAppName = FName(TEXT("GenericGraphEditorApp"));

struct FGenericGraphAssetEditorTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName ViewportID;
};

//////////////////////////////////////////////////////////////////////////

const FName FGenericGraphAssetEditorTabs::DetailsID(TEXT("Details"));
const FName FGenericGraphAssetEditorTabs::ViewportID(TEXT("Viewport"));

//////////////////////////////////////////////////////////////////////////

FAssetEditor_GenericGraph::FAssetEditor_GenericGraph()
{
	EditingGraph = nullptr;

	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FAssetEditor_GenericGraph::OnPackageSaved);
}

FAssetEditor_GenericGraph::~FAssetEditor_GenericGraph()
{
	UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
}

void FAssetEditor_GenericGraph::InitGenericGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UGenericGraph* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FEditorCommands_GenericGraph::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_GenericGraph(SharedThis(this)));
	}

	BindCommands();

	CreateInternalWidgets();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarBuilder->AddGenericGraphToolbar(ToolbarExtender);

	// Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_GenericGraphEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.225f)
					->AddTab(FGenericGraphAssetEditorTabs::ViewportID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(FGenericGraphAssetEditorTabs::DetailsID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, GenericGraphEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	RegenerateMenusAndToolbars();
}

void FAssetEditor_GenericGraph::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SoundCueEditor", "Sound Cue Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FAssetEditor_GenericGraph::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_GenericGraph::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_GenericGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::DetailsID);
}

FName FAssetEditor_GenericGraph::GetToolkitFName() const
{
	return FName("FGenericGraphEditor");
}

FText FAssetEditor_GenericGraph::GetBaseToolkitName() const
{
	return LOCTEXT("GenericGraphEditorAppLabel", "Generic Graph Editor");
}

FText FAssetEditor_GenericGraph::GetToolkitName() const
{
	const bool bDirtyState = EditingGraph->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("GenericGraphName"), FText::FromString(EditingGraph->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("GenericGraphEditorToolkitName", "{GenericGraphName}{DirtyState}"), Args);
}

FText FAssetEditor_GenericGraph::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

FLinearColor FAssetEditor_GenericGraph::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FAssetEditor_GenericGraph::GetWorldCentricTabPrefix() const
{
	return TEXT("GenericGraphEditor");
}

FString FAssetEditor_GenericGraph::GetDocumentationLink() const
{
	return TEXT("");
}

void FAssetEditor_GenericGraph::SaveAsset_Execute()
{
	if (EditingGraph != nullptr)
	{
		RebuildGenericGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_GenericGraph::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

TSharedRef<SDockTab> FAssetEditor_GenericGraph::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::ViewportID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if (ViewportWidget.IsValid())
	{
		SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FAssetEditor_GenericGraph::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::DetailsID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Details"))
		[
			PropertyWidget.ToSharedRef()
		];
}

void FAssetEditor_GenericGraph::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);

	//PropertyWidget->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FGenericGraphAssetEditor::IsPropertyEditable));
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_GenericGraph::OnFinishedChangingProperties);
}

TSharedRef<SGraphEditor> FAssetEditor_GenericGraph::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_GenericGraph", "Generic Graph");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_GenericGraph::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_GenericGraph::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}


void FAssetEditor_GenericGraph::BindCommands()
{
	ToolkitCommands->MapAction(FEditorCommands_GenericGraph::Get().GraphSettings,
		FExecuteAction::CreateSP(this, &FAssetEditor_GenericGraph::GraphSettings),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_GenericGraph::CanGraphSettings)
	);
}

void FAssetEditor_GenericGraph::CreateEdGraph()
{
	if (EditingGraph->EdGraph == nullptr)
	{
		EditingGraph->EdGraph = CastChecked<UEdGraph_GenericGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph_GenericGraph::StaticClass(), UAssetGraphSchema_GenericGraph::StaticClass()));
		EditingGraph->EdGraph->bAllowDeletion = false;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FAssetEditor_GenericGraph::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	// Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolkit>
	// however it should be safe, since commands are being used only within this editor
	// if it ever crashes, this function will have to go away and be reimplemented in each derived class

	GraphEditorCommands->MapAction(FEditorCommands_GenericGraph::Get().GraphSettings,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::GraphSettings),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanGraphSettings));

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanDuplicateNodes)
	);
}

TSharedPtr<SGraphEditor> FAssetEditor_GenericGraph::GetCurrGraphEditor()
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FAssetEditor_GenericGraph::GetSelectedNodes()
{
	FGraphPanelSelectionSet CurrentSelection;
	TSharedPtr<SGraphEditor> FocusedGraphEd = GetCurrGraphEditor();
	if (FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FAssetEditor_GenericGraph::RebuildGenericGraph()
{
	if (EditingGraph == nullptr)
	{
		LOG_WARNING(TEXT("FGenericGraphAssetEditor::RebuildGenericGraph EditingGraph is nullptr"));
		return;
	}

	UEdGraph_GenericGraph* EdGraph = Cast<UEdGraph_GenericGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	EdGraph->RebuildGenericGraph();
}

void FAssetEditor_GenericGraph::SelectAllNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}

bool FAssetEditor_GenericGraph::CanSelectAllNodes()
{
	return true;
}

void FAssetEditor_GenericGraph::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}

	LOG_WARNING(TEXT("FGenericGraphAssetEditor::DeleteSelectedNodes Exec"));
}

bool FAssetEditor_GenericGraph::CanDeleteNodes()
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node != nullptr && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	LOG_WARNING(TEXT("FGenericGraphAssetEditor::CanDeleteNodes Can't delete"));

	return false;
}

void FAssetEditor_GenericGraph::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FAssetEditor_GenericGraph::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FAssetEditor_GenericGraph::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FAssetEditor_GenericGraph::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FGenericPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FAssetEditor_GenericGraph::CanCopyNodes()
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_GenericGraph::PasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FAssetEditor_GenericGraph::PasteNodesHere(const FVector2D& Location)
{
}

bool FAssetEditor_GenericGraph::CanPasteNodes()
{
	return false;
}

void FAssetEditor_GenericGraph::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FAssetEditor_GenericGraph::CanDuplicateNodes()
{
	//return CanCopyNodes();
	return false;
}

void FAssetEditor_GenericGraph::GraphSettings()
{
	PropertyWidget->SetObject(EditingGraph);

	LOG_WARNING(TEXT("FGenericGraphAssetEditor::GraphSettings"));
}

bool FAssetEditor_GenericGraph::CanGraphSettings() const
{
	return true;
}

void FAssetEditor_GenericGraph::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.Num() == 0) 
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else if (Selection.Num() == 1)
	{
		PropertyWidget->SetObject(Selection[0]);
	}
	else
	{
		PropertyWidget->SetObject(nullptr);
	}
}

void FAssetEditor_GenericGraph::OnNodeDoubleClicked(UEdGraphNode* Node)
{
	
}

void FAssetEditor_GenericGraph::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph == nullptr)
		return;

	EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

void FAssetEditor_GenericGraph::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
	RebuildGenericGraph();
}

void FAssetEditor_GenericGraph::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager) 
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}


#undef LOCTEXT_NAMESPACE

