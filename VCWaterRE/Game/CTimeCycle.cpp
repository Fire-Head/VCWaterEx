#include "CTimeCycle.h"

float& CTimeCycle::m_fCurrentWaterRed = *(float *)0xA10740;
float& CTimeCycle::m_fCurrentWaterGreen = *(float *)0xA10734;
float& CTimeCycle::m_fCurrentWaterBlue = *(float *)0xA106A8;
float& CTimeCycle::m_fCurrentWaterAlpha = *(float *)0xA10688;


float& CTimeCycle::m_fCurrentAmbientRed = *(float *)0x978D18;
float& CTimeCycle::m_fCurrentAmbientGreen = *(float *)0xA0D8CC;
float& CTimeCycle::m_fCurrentAmbientBlue = *(float *)0xA0FCA8;


float& CTimeCycle::m_fCurrentDirectionalRed = *(float *)0x978D3C;
float& CTimeCycle::m_fCurrentDirectionalGreen = *(float *)0xA0D058;
float& CTimeCycle::m_fCurrentDirectionalBlue = *(float *)0xA0FC7C;