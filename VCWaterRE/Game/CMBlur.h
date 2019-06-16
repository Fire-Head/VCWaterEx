#pragma once
#include "..\address.h"
#include "types.h"
#include "RenderWare.h"

typedef Int32 FxType;

class CMBlur
{
public:
	static void AddRenderFx(RwCamera *camera, RwRect *rect, float z, FxType fx);
};