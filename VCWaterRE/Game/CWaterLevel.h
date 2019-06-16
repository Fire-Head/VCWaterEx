#pragma once
#include "..\address.h"
#include "CVector.h"


struct ColData
{
	UInt8 SurfaceType;
	UInt8 PieceType;
};

class CWaterLevel
{
public:
	static void Initialise(char *datFile);
	static void Shutdown();
	static Bool GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ);
	static Bool GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel);
	static CVector GetWaterNormal(float fX, float fY);
	static Bool GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance);
	static Bool GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel);
	static void RenderWater();
	static void RenderTransparentWater(void);
	static float CalcDistanceToWater(float fX, float fY);
	static void PreCalcWaterGeometry(void);
	
	static void AllocateBoatWakeArray()
	{ }
	static void FreeBoatWakeArray()
	{ }
	
	static Bool &m_bRenderSeaBed;
	static Int32 &m_nRenderWaterLayers;
};