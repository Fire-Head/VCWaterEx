#include "CWaterLevel.h"

Bool &CWaterLevel::m_bRenderSeaBed = *(Bool*)0xA10B86;
Int32 &CWaterLevel::m_nRenderWaterLayers = *(Int32*)0xA10AF2;

// Converted from static void __cdecl CWaterLevel::Initialise(char *datFile) 0x5C3940
void CWaterLevel::Initialise(char* datFile)
{
	((void (__cdecl *)(char*))0x5C3940)(datFile);
}

// Converted from static void __cdecl CWaterLevel::Shutdown() 0x5C3880
void CWaterLevel::Shutdown()
{
	((void (__cdecl *)())0x5C3880)();
}

// Converted from static Bool __cdecl CWaterLevel::GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ) 0x5C2C80
Bool CWaterLevel::GetWaterLevel(float fX, float fY, float fZ, float* pfOutLevel, Bool bDontCheckZ)
{
	return ((Bool (__cdecl *)(float, float, float, float*, Bool))0x5C2C80)(fX, fY, fZ, pfOutLevel, bDontCheckZ);
}

// Converted from static Bool __cdecl CWaterLevel::GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel) 0x5C2BE0
Bool CWaterLevel::GetWaterLevelNoWaves(float fX, float fY, float fZ, float* pfOutLevel)
{
	return ((Bool (__cdecl *)(float, float, float, float*))0x5C2BE0)(fX, fY, fZ, pfOutLevel);
}

// Converted from static CVector __cdecl CWaterLevel::GetWaterNormal(float fX, float fY) 0x5C2A20
CVector CWaterLevel::GetWaterNormal(float fX, float fY)
{
	return ((CVector (__cdecl *)(float, float))0x5C2A20)(fX, fY);
}

// Converted from static Bool __cdecl CWaterLevel::GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance) 0x5BCD00
Bool CWaterLevel::GetGroundLevel(CVector const& vecPosn, float* pfOutLevel, ColData* pData, float fDistance)
{
	return ((Bool (__cdecl *)(CVector const&, float*, ColData*, float))0x5BCD00)(vecPosn, pfOutLevel, pData, fDistance);
}

// Converted from static Bool __cdecl CWaterLevel::GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel) 0x5BCBA0
Bool CWaterLevel::GetWaterDepth(CVector const& vecPosn, float* pfDepth, float* pfLevelNoWaves, float* pfGroundLevel)
{
	return ((Bool (__cdecl *)(CVector const&, float*, float*, float*))0x5BCBA0)(vecPosn, pfDepth, pfLevelNoWaves, pfGroundLevel);
}

// Converted from static void __cdecl CWaterLevel::RenderWater() 0x5C1710
void CWaterLevel::RenderWater()
{
	((void (__cdecl *)())0x5C1710)();
}

// Converted from static void __cdecl CWaterLevel::RenderTransparentWater() 0x5BFF00
void CWaterLevel::RenderTransparentWater()
{
	((void (__cdecl *)())0x5BFF00)();
}

// Converted from static float __cdecl CWaterLevel::CalcDistanceToWater(float fX, float fY) 0x5BCE00
float CWaterLevel::CalcDistanceToWater(float fX, float fY)
{
	return ((float (__cdecl *)(float, float))0x5BCE00)(fX, fY);
}

void CWaterLevel::PreCalcWaterGeometry(void)
{
	((void (__cdecl *)())0x5BF540)();
}