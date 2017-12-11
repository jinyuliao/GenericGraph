#include "GenericGraphAssetEditor/Settings_GenericGraphEditor.h"

UGenericGraphEditorSettings::UGenericGraphEditorSettings()
{
	AutoLayoutStrategy = EAutoLayoutStrategy::Tree;

	bFirstPassOnly = false;

	bRandomInit = false;

	OptimalDistance = 100.f;

	MaxIteration = 50;

	InitTemperature = 10.f;

	CoolDownRate = 10.f;
}

UGenericGraphEditorSettings::~UGenericGraphEditorSettings()
{

}

