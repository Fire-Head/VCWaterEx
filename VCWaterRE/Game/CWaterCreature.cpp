#include "CWaterCreatures.h"


void CWaterCreatures::UpdateAll(void)
{
	((void (__cdecl *)())0x636EC0)();
}

bool CWaterCreatures::IsSpaceForMoreWaterCreatures(void)
{
	return ((bool (__cdecl *)())0x636D10)();
}

void CWaterCreatures::CreateOne(CVector const&pos, int id)
{
	((void (__cdecl *)(CVector const&, int))0x637200)(pos, id);
}