#include "CClock.h"

unsigned char& CClock::ms_nGameClockHours = *(unsigned char*)0xA10B6B;
unsigned char& CClock::ms_nGameClockMinutes = *(unsigned char*)0xA10B92;
unsigned short& CClock::ms_nGameClockSeconds = *(unsigned short*)0xA10A3C;