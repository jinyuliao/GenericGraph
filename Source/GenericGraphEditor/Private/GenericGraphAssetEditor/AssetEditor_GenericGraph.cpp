#include "GenericGraphAssetEditor/AssetEditor_GenericGraph.h"
#include "GenericGraphEditorPCH.h"
#include "GenericGraphAssetEditor/AssetEditorToolbar_GenericGraph.h"
#include "GenericGraphAssetEditor/AssetGraphSchema_GenericGraph.h"
#include "GenericGraphAssetEditor/EditorCommands_GenericGraph.h"
#include "GenericGraphAssetEditor/EdGraph_GenericGraph.h"
#include "AssetToolsModule.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EdGraphUtilities.h"
#include "GenericGraphAssetEditor/EdGraph_GenericGraph.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/EdNode_GenericGraphEdge.h"
#include "AutoLayout/TreeLayoutStrategy.h"
#include "AutoLayout/ForceDirectedLayoutStrategy.h"

#define LOCTEXT_NAMESPACE "AssetEditor_GenericGraph"

const FName GenericGraphEditorAppName = FName(TEXT("GenericGraphEditorApp"));

struct FGenericGraphAssetEditorTabs
{
	// Tab identifiers
	static const FName GenericGraphPropertyID;
	static const FName ViewportID;
	static const FName GenericGraphEditorSettingsID;
};

//////////////////////////////////////////////////////////////////////////

const FName FGenericGraphAssetEditorTabs::GenericGraphPropertyID(TEXT("GenericGraphProperty"));
const FName FGenericGraphAssetEditorTabs::ViewportID(TEXT("Viewport"));
const FName FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID(TEXT("GenericGraphEditorSettings"));

//////////////////////////////////////////////////////////////////////////

FAssetEditor_GenericGraph::FAssetEditor_GenericGraph()
{
	EditingGraph = nullptr;

	GenricGraphEditorSettings = NewObject<UGenericGraphEditorSettings>(UGenericGraphEditorSettings::StaticClass());

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
					->SetSizeCoefficient(0.65f)
					->AddTab(FGenericGraphAssetEditorTabs::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FGenericGraphAssetEditorTabs::GenericGraphPropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID, ETabState::OpenedTab)
					)
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
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_GenericGraphEditor", "Generic Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FAssetEditor_GenericGraph::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphPropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_GenericGraph::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_GenericGraph::SpawnTab_EditorSettings))
		.SetDisplayName(LOCTEXT("EditorSettingsTab", "Generic Graph Editor Setttings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_GenericGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphPropertyID);
	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID);
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

UGenericGraphEditorSettings* FAssetEditor_GenericGraph::GetSettings() const
{
	return GenricGraphEditorSettings;
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
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::GenericGraphPropertyID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Property"))
		[
			PropertyWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FAssetEditor_GenericGraph::SpawnTab_EditorSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::GenericGraphEditorSettingsID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("EditorSettings_Title", "Generic Graph Editor Setttings"))
		[
			EditorSettingsWidget.ToSharedRef()
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
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_GenericGraph::OnFinishedChangingProperties);

	EditorSettingsWidget = PropertyModule.CreateDetailView(Args);
	EditorSettingsWidget->SetObject(GenricGraphEditorSettings);
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

	ToolkitCommands->MapAction(FEditorCommands_GenericGraph::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FAssetEditor_GenericGraph::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_GenericGraph::CanAutoArrange)
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

	GraphEditorCommands->MapAction(FEditorCommands_GenericGraph::Get().AutoArrange,
		FExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::AutoArrange),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_GenericGraph::CanAutoArrange));

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

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FAssetEditor_GenericGraph::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_GenericGraph::CanRenameNodes)
	);
}

TSharedPtr<SGraphEditor> FAssetEditor_GenericGraph::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FAssetEditor_GenericGraph::GetSelectedNodes() const
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
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;;

		if (UEdNode_GenericGraphNode* EdNode_Node = Cast<UEdNode_GenericGraphNode>(EdNode))
		{
			EdNode_Node->Modify();

			const UEdGraphSchema* Schema = EdNode_Node->GetSchema();
			if (Schema != nullptr)
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
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

		if (UEdNode_GenericGraphEdge* EdNode_Edge = Cast<UEdNode_GenericGraphEdge>(*SelectedIter))
		{
			UEdNode_GenericGraphNode* StartNode = EdNode_Edge->GetStartNode();
			UEdNode_GenericGraphNode* EndNode = EdNode_Edge->GetEndNode();

			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
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
	// Find the graph editor with focus
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}
	// Select the newly pasted stuff
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();

	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		// Clear the selection set (newly pasted stuff will be selected)
		CurrentGraphEditor->ClearSelectionSet();

		// Grab the text to paste from the clipboard.
		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		// Import the nodes
		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		//Average position of nodes so we can move them while still maintaining relative distances to each other
		FVector2D AvgNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FAssetEditor_GenericGraph::CanPasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FAssetEditor_GenericGraph::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FAssetEditor_GenericGraph::CanDuplicateNodes()
{
	return CanCopyNodes();
}

void FAssetEditor_GenericGraph::GraphSettings()
{
	PropertyWidget->SetObject(EditingGraph);
}

bool FAssetEditor_GenericGraph::CanGraphSettings() const
{
	return true;
}

void FAssetEditor_GenericGraph::AutoArrange()
{
	UEdGraph_GenericGraph* EdGraph = Cast<UEdGraph_GenericGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	const FScopedTransaction Transaction(LOCTEXT("GenericGraphEditorAutoArrange", "Generic Graph Editor: Auto Arrange"));

	EdGraph->Modify();

	UAutoLayoutStrategy* LayoutStrategy = nullptr;
	switch (GenricGraphEditorSettings->AutoLayoutStrategy)
	{
	case EAutoLayoutStrategy::Tree:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UTreeLayoutStrategy::StaticClass());
		break;
	case EAutoLayoutStrategy::ForceDirected:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UForceDirectedLayoutStrategy::StaticClass());
		break;
	default:
		break;
	}

	if (LayoutStrategy != nullptr)
	{
		LayoutStrategy->Settings = GenricGraphEditorSettings;
		LayoutStrategy->Layout(EdGraph);
		LayoutStrategy->ConditionalBeginDestroy();
	}
	else
	{
		LOG_ERROR(TEXT("FAssetEditor_GenericGraph::AutoArrange LayoutStrategy is null."));
	}
}

bool FAssetEditor_GenericGraph::CanAutoArrange() const
{
	return EditingGraph != nullptr && Cast<UEdGraph_GenericGraph>(EditingGraph->EdGraph) != nullptr;
}

void FAssetEditor_GenericGraph::OnRenameNode()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FAssetEditor_GenericGraph::CanRenameNodes() const
{
	UEdGraph_GenericGraph* EdGraph = Cast<UEdGraph_GenericGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	UGenericGraph* Graph = EdGraph->GetGenericGraph();
	check(Graph != nullptr)

	return Graph->bCanRenameNode && GetSelectedNodes().Num() == 1;
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
	else
	{
		PropertyWidget->SetObjects(Selection);
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

