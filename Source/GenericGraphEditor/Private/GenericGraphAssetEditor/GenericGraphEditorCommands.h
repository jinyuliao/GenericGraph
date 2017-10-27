#pragma once

#include "CoreMinimal.h"

class FGenericGraphEditorCommands : public TCommands<FGenericGraphEditorCommands>
{
public:
	/** Constructor */
	FGenericGraphEditorCommands()
		: TCommands<FGenericGraphEditorCommands>("GenericGraphEditor", NSLOCTEXT("Contexts", "GenericGraphEditor", "Generic Graph Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> GraphSettings;

	virtual void RegisterCommands() override;
};
