#pragma once

#include "CoreMinimal.h"

class GENERICGRAPHEDITOR_API FEditorCommands_GenericGraph : public TCommands<FEditorCommands_GenericGraph>
{
public:
	/** Constructor */
	FEditorCommands_GenericGraph()
		: TCommands<FEditorCommands_GenericGraph>("GenericGraphEditor", NSLOCTEXT("Contexts", "GenericGraphEditor", "Generic Graph Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;

	virtual void RegisterCommands() override;
};
