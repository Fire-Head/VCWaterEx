#pragma once
#include "..\address.h"

class CTimer
{
public:
	static bool& m_CodePause;
	static bool& m_UserPause;
	static unsigned int& m_FrameCounter;
	static float& ms_fTimeStep;

	static unsigned int& m_snTimeInMilliseconds;
};