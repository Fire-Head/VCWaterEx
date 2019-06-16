#include "CBoat.h"


float &CBoat::WAKE_LIFETIME = *(float *)0x69AE68;
float &CBoat::MIN_WAKE_INTERVAL = *(float *)0x69AE64;
CBoat **CBoat::apFrameWakeGeneratingBoats = (CBoat **)0x9381F8;

void CBoat::FillBoatList(void)
{
	((void (__cdecl *)())0x59F360)();
}

