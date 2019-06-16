#pragma once
#include "_precl.h"
#include "_config.h"


class IWaterLevel
{
public:
	virtual void Initialise(char *datFile) = 0;
	virtual void Shutdown() = 0;
	
	virtual Bool GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ) = 0;
	virtual Bool GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel) = 0;
	virtual CVector GetWaterNormal(float fX, float fY) = 0;
	virtual Bool GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance) = 0;
	virtual Bool GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel) = 0;
	
	virtual void RenderWater() = 0;
	virtual void RenderTransparentWater(void) = 0;
	virtual float CalcDistanceToWater(float fX, float fY) = 0;
	
	virtual void PreCalcWaterGeometry(void) = 0;
	
	virtual void NoSeaBedCheat() = 0;
	virtual void RenderWaterLayersCheat() = 0;
	
	virtual void AllocateBoatWakeArray() = 0;
	virtual void FreeBoatWakeArray() = 0;
};

template<typename C>
class WaterLevel_FromStatic : public IWaterLevel
{
public:
	void Initialise(char *datFile)
	{	C::Initialise(datFile);	}
	
	void Shutdown()
	{	C::Shutdown();	}
	
	Bool GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ)
	{	return C::GetWaterLevel(fX, fY, fZ, pfOutLevel, bDontCheckZ);	}
	
	Bool GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel)
	{	return C::GetWaterLevelNoWaves(fX, fY, fZ, pfOutLevel);	}
	
	CVector GetWaterNormal(float fX, float fY)
	{	return C::GetWaterNormal(fX, fY);	}
	
	Bool GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance)
	{	return C::GetGroundLevel(vecPosn, pfOutLevel, pData, fDistance);	}
	
	Bool GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel)
	{	return C::GetWaterDepth(vecPosn, pfDepth, pfLevelNoWaves, pfGroundLevel);	}
	
	void RenderWater()
	{	C::RenderWater();	}
	
	void RenderTransparentWater(void)
	{	C::RenderTransparentWater();	}
	
	float CalcDistanceToWater(float fX, float fY)
	{	return C::CalcDistanceToWater(fX, fY);	}
	
	void PreCalcWaterGeometry(void)
	{	C::PreCalcWaterGeometry();	}
	
	void NoSeaBedCheat()
	{
		C::m_bRenderSeaBed ^= true;
	}
	
	void RenderWaterLayersCheat()
	{
		if ( ++C::m_nRenderWaterLayers > 5 )
			C::m_nRenderWaterLayers = 0;
	}
	
	void AllocateBoatWakeArray()
	{	C::AllocateBoatWakeArray();	}
	
	void FreeBoatWakeArray()
	{	C::FreeBoatWakeArray();	}
};

class CWaterLevelEx
{
public:
	static void Patch(Bool bPatch);
	
	static IWaterLevel *Get();

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
	
	static void NoSeaBedCheat();
	static void RenderWaterLayersCheat();
	
	static void AllocateBoatWakeArray();
	static void FreeBoatWakeArray();
};