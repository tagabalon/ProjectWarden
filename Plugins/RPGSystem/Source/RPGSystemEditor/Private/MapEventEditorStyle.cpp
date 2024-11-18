#include "MapEventEditorStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> FMapEventEditorStyle::StyleSet = nullptr;

FName FMapEventEditorStyle::GetStyleSetName()
{
	static FName StyleName(TEXT("MapEventEditorStyle"));
	return StyleName;
}

void FMapEventEditorStyle::Initialize()
{
	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("MapEventEditorStyle")));

	const FVector2D Icon32(32.0f, 32.0f);
	const FVector2D Icon128(128.0f, 128.0f);

	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("RPGSystem"))->GetBaseDir() / TEXT("Resources"));
	StyleSet->Set("ClassIcon.MapEvent", new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Icons/MapEventIcon"), TEXT(".png")), Icon32));
	StyleSet->Set("ClassThumbnail.MapEvent", new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Icons/MapEventThumbnail"), TEXT(".png")), Icon128));

	StyleSet->Set("CommandNode.Body", new FSlateBoxBrush(StyleSet->RootToContentDir(TEXT("Nodes/NodeBody"), TEXT(".png")), FMargin(16.f / 64.f)));

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FMapEventEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
	ensure(StyleSet.IsUnique());
	StyleSet.Reset();
}