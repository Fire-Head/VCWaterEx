#include "CMBlur.h"


void CMBlur::AddRenderFx(RwCamera *camera, RwRect *rect, float z, FxType fx)
{
	((void (__cdecl *)(RwCamera *, RwRect *, float, FxType))0x55D160)(camera, rect, z, fx);
}