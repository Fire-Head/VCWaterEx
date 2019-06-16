#include "CTimer.h"


bool& CTimer::m_CodePause = *(bool *)0xA10B76;
bool& CTimer::m_UserPause = *(bool *)0xA10B36;
unsigned int& CTimer::m_FrameCounter = *(unsigned int *)0xA0D898;
float& CTimer::ms_fTimeStep = *(float *)0x975424;
unsigned int& CTimer::m_snTimeInMilliseconds = *(unsigned int *)0x974B2C;