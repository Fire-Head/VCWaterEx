#include "CGeneral.h"


int rand(void)
{
	return ((int (__cdecl *)())0x6499F0)();
}

#define RAND_MAX		32767

// Converted from static float __cdecl CGeneral::GetRandomNumberInRange(float min, float max) 0x054A520
float CGeneral::GetRandomNumberInRange(float min, float max)
{
	return ((Float)(UInt16)GetRandomNumber() / Float(RAND_MAX + 1)) * (max - min) + min;
}

// Converted from static int __cdecl CGeneral::GetRandomNumberInRange(int min, int max) 0x54A4C0
int CGeneral::GetRandomNumberInRange(int min, int max)
{
	return ((Float)(UInt16)GetRandomNumber() / Float(RAND_MAX + 1)) * (max - min) + min;
}

int CGeneral::GetRandomNumber()
{
	return rand() & 0xFFFF;
}