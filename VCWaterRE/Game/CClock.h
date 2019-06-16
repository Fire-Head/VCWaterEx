#pragma once

#include "..\address.h"

class CClock
{
public:
   	static unsigned char& ms_nGameClockHours;
	static unsigned char& ms_nGameClockMinutes;
	static unsigned short& ms_nGameClockSeconds;
};