#include "CGame.h"

int &CGame::currArea = *(int*)0x978810;


bool CGame::CanSeeWaterFromCurrArea()
{
	return ((bool (__cdecl *)())0x4A4360)();
}