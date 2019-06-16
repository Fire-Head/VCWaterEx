#pragma once
#include "..\address.h"

class CTimeCycle
{
public:
	static float& m_fCurrentWaterRed;
	static float& m_fCurrentWaterGreen;
	static float& m_fCurrentWaterBlue;
	static float& m_fCurrentWaterAlpha;
	
	static float& m_fCurrentAmbientRed;
	static float& m_fCurrentAmbientGreen;
	static float& m_fCurrentAmbientBlue;
	
	static float& m_fCurrentDirectionalRed;
	static float& m_fCurrentDirectionalGreen;
	static float& m_fCurrentDirectionalBlue;
};