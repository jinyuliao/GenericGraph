#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphAssetEditor.h"
#include "GenericGraphAssetGraphSchema.h"

#define LOCTEXT_NAMESPACE "GenericGraphAssetEditor"

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

FGenericGraphAssetEditor::FGenericGraphAssetEditor()
{
	EditingGraph = nullptr;
	EditingEdGraph = nullptr;
}

FGenericGraphAssetEditor::~FGenericGraphAssetEditor()
{

}

void FGenericGraphAssetEditor::InitGenericGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UGenericGraph* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGraphEditorCommands::Register();

	BindCommands();

	CreateInternalWidgets();

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

	ExtendToolbar();
	RegenerateMenusAndToolbars();
}

void FGenericGraphAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SoundCueEditor", "Sound Cue Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FGenericGraphAssetEditor::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FGenericGraphAssetEditorTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FGenericGraphAssetEditor::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FGenericGraphAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FGenericGraphAssetEditorTabs::DetailsID);
}

FName FGenericGraphAssetEditor::GetToolkitFName() const
{
	return FName("FGenericGraphEditor");
}

FText FGenericGraphAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("GenericGraphEditorAppLabel", "Generic Graph Editor");
}

FText FGenericGraphAssetEditor::GetToolkitName() const
{
	const bool bDirtyState = EditingGraph->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("GenericGraphName"), FText::FromString(EditingGraph->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("GenericGraphEditorToolkitName", "{GenericGraphName}{DirtyState}"), Args);
}

FText FGenericGraphAssetEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

FLinearColor FGenericGraphAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FGenericGraphAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("GenericGraphEditor");
}

FString FGenericGraphAssetEditor::GetDocumentationLink() const
{
	return TEXT("");
}

void FGenericGraphAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
}

TSharedRef<SDockTab> FGenericGraphAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs& Args)
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

TSharedRef<SDockTab> FGenericGraphAssetEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FGenericGraphAssetEditorTabs::DetailsID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Details"))
		[
			PropertyWidget.ToSharedRef()
		];
}

void FGenericGraphAssetEditor::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
}

TSharedRef<SGraphEditor> FGenericGraphAssetEditor::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_GenericGraph", "Generic Graph");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FGenericGraphAssetEditor::OnSelectedNodesChanged);
	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FGenericGraphAssetEditor::OnNodeTitleCommitted);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FGenericGraphAssetEditor::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingEdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FGenericGraphAssetEditor::ExtendToolbar()
{

}

void FGenericGraphAssetEditor::BindCommands()
{

}

void FGenericGraphAssetEditor::CreateEdGraph()
{
	EditingEdGraph = CastChecked<UEdGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph::StaticClass(), UGenericGraphAssetGraphSchema::StaticClass()));
	EditingEdGraph->bAllowDeletion = false;

	// Give the schema a chance to fill out any required nodes (like the results node)
	const UEdGraphSchema* Schema = EditingEdGraph->GetSchema();
	Schema->CreateDefaultNodesForGraph(*EditingEdGraph);
}

void FGenericGraphAssetEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{

}

void FGenericGraphAssetEditor::OnNodeDoubleClicked(UEdGraphNode* Node)
{

}

void FGenericGraphAssetEditor::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged)
{

}

#undef LOCTEXT_NAMESPACE
