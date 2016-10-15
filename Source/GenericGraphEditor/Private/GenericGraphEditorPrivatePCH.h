// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "CoreUObject.h"
#include "UnrealEd.h"
#include "ScopedTransaction.h"

#include "Kismet2/BlueprintEditorUtils.h"

#include "GraphEditor.h"
#include "GraphEditorActions.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphUtilities.h"
#include "GraphEditorSettings.h"

#include "GenericCommands.h"

#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"

#include "SGraphNode.h"
#include "SGraphPin.h"
#include "SInlineEditableTextBlock.h"
#include "SCommentBubble.h"
#include "SLevelOfDetailBranchNode.h"

#include "SDockTab.h"
#include "SNodePanel.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "EdGraph/EdGraphSchema.h"

#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/AssetEditorManager.h"
#include "Editor/EditorWidgets/Public/ITransportControl.h"

#include "GenericGraph.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "IGenericGraphEditor.h"

#define LOG_WARNING(FMT, ...) UE_LOG(GenericGraphEditor, Warning, (FMT), ##__VA_ARGS__)
