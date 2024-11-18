
#include "Asset/MapEventEditorContext.h"

#include "Asset/MapEventEditor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MapEventEditorContext)

UMapEvent* UMapEventEditorContext::GetMapEvent() const
{
	return MapEventEditor.IsValid() ? MapEventEditor.Pin()->GetMapEvent() : nullptr;
}
