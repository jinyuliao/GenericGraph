#include "GenericGraphEditorPrivatePCH.h"
#include "GenericGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "GenericGraphEditorCommands"

void FGenericGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
