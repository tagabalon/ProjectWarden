#pragma once

#include "GraphEditor.h"

#include "MapEvent.h"

class FMapEventEditor;
class IDetailsView;

class RPGSYSTEMEDITOR_API SMapEventGraphEditor : public SGraphEditor
{
public:
	SLATE_BEGIN_ARGS(SMapEventGraphEditor)
	{}
		SLATE_ARGUMENT(TSharedPtr<IDetailsView>, DetailsView)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedPtr<FMapEventEditor> InMapEventEditor);

	FOnSelectionChanged OnSelectionChangedEvent;

	virtual FGraphAppearanceInfo GetGraphAppearanceInfo() const;
	virtual FText GetCornerText() const;

protected:
	virtual void OnSelectionChanged(const TSet<UObject*>& Nodes);

	TWeakObjectPtr<UMapEvent> MapEvent;
	TWeakPtr<FMapEventEditor> MapEventEditor;
	TSharedPtr<IDetailsView> DetailsView;

};
