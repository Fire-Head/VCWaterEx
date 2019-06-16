#include "CCullZones.h"


bool CCullZones::CamNoRain(void)
{
	return ((bool (__cdecl *)())0x57E0E0)();
}

bool CCullZones::PlayerNoRain(void)
{
	return ((bool (__cdecl *)())0x57E0C0)();
}

bool CCullZones::WaterFudge(void)
{
	return ((bool (__cdecl *)())0x57E100)();
}