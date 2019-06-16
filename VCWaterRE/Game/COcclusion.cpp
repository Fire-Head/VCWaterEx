#include "COcclusion.h"

bool COcclusion::IsAABoxOccluded(CVector pos, float fSizeX, float fSizeY, float fSizeZ)
{
	return ((bool (__cdecl *)(CVector, float, float, float))0x634150)(pos, fSizeX, fSizeY, fSizeZ);
}