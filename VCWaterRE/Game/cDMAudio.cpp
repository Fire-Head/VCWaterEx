#include "cDMAudio.h"

void cDMAudio::PlayFrontEndSound(unsigned short v1, unsigned int v2)
{
	((void (__thiscall *)(cDMAudio *, unsigned short, unsigned int))0x5F9960)(this, v1, v2);
}

void cDMAudio::PlayOneShot(int v1, unsigned short v2, float v3)
{
	((void (__thiscall *)(cDMAudio *, int, unsigned short, float))0x5F9DA0)(this, v1, v2, v3);
}


cDMAudio &DMAudio = *(cDMAudio*)0xA10B8A;