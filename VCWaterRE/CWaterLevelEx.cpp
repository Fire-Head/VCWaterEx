#include "CWaterLevelEx.h"
#include "settings.h"
#include "waterlevel.h"
#include "waterlevelps2.h"
#include "waterlevelGTA3.h"
#include "inc/CPatch.h"
#include "tweak.h"

class CWaterLevelPC : public WaterLevel_FromStatic<CWaterLevel>
{
}WaterLevelPC;

class CWaterLevelFH : public WaterLevel_FromStatic<FH::CWaterLevel>
{
}WaterLevelFH;

class CWaterLevelFHPS2 : public WaterLevel_FromStatic<FHPS2::CWaterLevel>
{
}WaterLevelFHPS2;


class CWaterLevelFHGTA3 : public WaterLevel_FromStatic<FHGTA3::CWaterLevel>
{
}WaterLevelFHGTA3;

IWaterLevel *apWaterLevels[] =
{
	&WaterLevelPC,
	&WaterLevelFH,
	&WaterLevelFHPS2,
	&WaterLevelFHGTA3,
};

void CWaterLevelEx::Patch(Bool bOriginal)
{
	struct tSavedPatchData
	{
		bool patched;
		char code;
		int addr;
		
		int place;
		void *ptr;
		
		
		tSavedPatchData(int nplace, void *fptr):
			patched(false),
			place(nplace),
			ptr(fptr)
		{ }
		
		void Set()
		{
			if ( !patched )
			{
				CPatch::GetChar(place, &code);
				CPatch::GetInt(place+1, &addr);
				CPatch::RedirectJump(place, ptr);
				patched = true;
			}
		}
		
		void Reset()
		{
			if ( patched )
			{
				CPatch::SetChar(place, code);
				CPatch::SetInt(place+1, addr);
				patched = false;
			}
		}
	};
	
	static tSavedPatchData spatches[] =
	{
		tSavedPatchData(0x5C2C80, GetWaterLevel),
		tSavedPatchData(0x5C2BE0, GetWaterLevelNoWaves),
		tSavedPatchData(0x5C2A20, GetWaterNormal),
		tSavedPatchData(0x5BCE00, CalcDistanceToWater),
		tSavedPatchData(0x5BCD00, GetGroundLevel),
		tSavedPatchData(0x5BCBA0, GetWaterDepth)
	};
	
#if 0
	struct tSavedByte
	{
		bool patched;
		char code;
		int place;
		
		
		tSavedByte(int nplace):
			patched(false),
			place(nplace)
		{ }
		
		void Set()
		{
			if ( !patched )
			{
				CPatch::GetChar(place, &code);
				CPatch::SetChar(place, 0xC3);
				patched = true;
			}
		}
		
		void Reset()
		{
			if ( patched )
			{
				CPatch::SetChar(place, code);
				patched = false;
			}
		}
	};
	
	// just to make sure that original water is completely disabled
	static tSavedByte stst[] =
	{
		tSavedByte(0x5BCBA0),// CWaterLevel::GetWaterDepth
		tSavedByte(0x5BCD00),// CWaterLevel::GetGroundLevel	
		tSavedByte(0x5BCDA0),// CWaterLevel::RenderAndEmptyRenderBuffer	
		tSavedByte(0x5BCE00),// CWaterLevel::CalcDistanceToWater	
		tSavedByte(0x5BD0A0),// CWaterLevel::RenderOneFlatSmallWaterPolyBlended	
		tSavedByte(0x5BD740),// CWaterLevel::RenderWakeSegment	
		tSavedByte(0x5BE2F0),// CWaterLevel::RenderBoatWakes	
		tSavedByte(0x5BE6D0),// CWaterLevel::PreCalcWavyMask	
		tSavedByte(0x5BF0E0),// CWaterLevel::PreCalcWavySector	
		tSavedByte(0x5BF540),// CWaterLevel::PreCalcWaterGeometry	
		tSavedByte(0x5BF9E0),// CWaterLevel::RenderOneFlatExtraHugeWaterPoly	
		tSavedByte(0x5BFC70),// CWaterLevel::RenderOneFlatHugeWaterPoly	
		tSavedByte(0x5BFF00),// CWaterLevel::RenderTransparentWater	
		tSavedByte(0x5C1710),// CWaterLevel::RenderWater	
		tSavedByte(0x5C2A20),// CWaterLevel::GetWaterNorma	
		tSavedByte(0x5C2BE0),// CWaterLevel::GetWaterLevelNoWaves	
		tSavedByte(0x5C2C80) // CWaterLevel::GetWaterLevel	
	};
#endif
	
	if ( bOriginal )
	{
#if 0
		for ( int i = 0; i < ARRLEN(stst); i++ )
			stst[i].Reset();
#endif

		for ( int i = 0; i < ARRLEN(spatches); i++ )
			spatches[i].Reset();
	}
	else
	{
		for ( int i = 0; i < ARRLEN(spatches); i++ )
			spatches[i].Set();
		
#if 0
		for ( int i = 0; i < ARRLEN(stst); i++ )
			stst[i].Set();
#endif
	}
}

IWaterLevel *CWaterLevelEx::Get()
{
	return apWaterLevels[g_Switch];
}

void CWaterLevelEx::Initialise(char *datFile)
{
#ifndef _USE_ORIGINAL_WATER_VARS
	for ( Int32 i = 0; i < WATER_MAX; i++ )
		apWaterLevels[i]->Initialise(datFile);
#else
	apWaterLevels[WATER_ORIGINAL]->Initialise(datFile);
#endif
}

void CWaterLevelEx::Shutdown()
{
#ifndef _USE_ORIGINAL_WATER_VARS
	for ( Int32 i = 0; i < WATER_MAX; i++ )
		apWaterLevels[i]->Shutdown();
#else
	apWaterLevels[WATER_ORIGINAL]->Shutdown();
#endif
}

Bool CWaterLevelEx::GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ)
{
#if 0
	if ( false )
	{
		Float wa;
		Float wb;
		if ( apWaterLevels[WATER_ORIGINAL]->GetWaterLevel(fX, fY, fZ, &wa, bDontCheckZ)
			&& apWaterLevels[WATER_FH]->GetWaterLevel(fX, fY, fZ, &wb, bDontCheckZ) )
		{
			if ( wa != wb )
			{
				printf("GetWaterLevel fucked up %f, %f\n", wa, wb);
			}
		}
	}
#endif
	
	return Get()->GetWaterLevel(fX, fY, fZ, pfOutLevel, bDontCheckZ);
}

Bool CWaterLevelEx::GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel)
{
#if 0
	if ( false )
	{
		Float wa;
		Float wb;
		if ( apWaterLevels[WATER_ORIGINAL]->GetWaterLevelNoWaves(fX, fY, fZ, &wa)
			&& apWaterLevels[WATER_FH]->GetWaterLevelNoWaves(fX, fY, fZ, &wb) )
		{
			if ( wa != wb )
				printf("GetWaterLevelNoWaves fucked up, %f, %f\n", wa, wb);
		}
	}
#endif
		
	return Get()->GetWaterLevelNoWaves(fX, fY, fZ, pfOutLevel);
}

CVector CWaterLevelEx::GetWaterNormal(float fX, float fY)
{
#if 0
	if ( false )
	{
		CVector a = apWaterLevels[WATER_ORIGINAL]->GetWaterNormal(fX, fY);
		CVector b = apWaterLevels[WATER_FH]->GetWaterNormal(fX, fY);
		
		if ( a.x != b.x || a.y != b.y || a.z != b.z )
			printf("GetWaterNormal fucked up, %f,%f,%f  %f,%f,%f\n", a.x, a.y, a.z, b.x, b.y, b.z);
	}
#endif
	
	return Get()->GetWaterNormal(fX, fY);
}

Bool CWaterLevelEx::GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance)
{
#if 0
	if ( false )
	{
		Float gla;
		Float glb;
		
		if ( apWaterLevels[WATER_ORIGINAL]->GetGroundLevel(vecPosn, &gla, NULL, fDistance)
			&& apWaterLevels[WATER_FH]->GetGroundLevel(vecPosn, &glb, NULL, fDistance) )
		{
			if ( gla != glb )
				printf("GetGroundLevel fucked up %f %f\n", gla, glb);
		}
	}
#endif
	
	return Get()->GetGroundLevel(vecPosn, pfOutLevel, pData, fDistance);
}

Bool CWaterLevelEx::GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel)
{
#if 0
	if ( false )
	{
		Float da, lnwa, ga;
		Float db, lnwb, gb;
		if ( apWaterLevels[WATER_ORIGINAL]->GetWaterDepth(vecPosn, &da, &lnwa, &ga)
			&& apWaterLevels[WATER_FH]->GetWaterDepth(vecPosn, &db, &lnwb, &gb) )
		{
			if ( da != db || lnwa != lnwb || ga != gb )
				printf("GetWaterDepth fucked up %f,%f,%f %f,%f,%f\n", da, lnwa, ga, db, lnwb, gb);
		}
	}
#endif
	
	return Get()->GetWaterDepth(vecPosn, pfDepth, pfLevelNoWaves, pfGroundLevel);
}

void CWaterLevelEx::RenderWater()
{
	Get()->RenderWater();
}

void CWaterLevelEx::RenderTransparentWater(void)
{
	Get()->RenderTransparentWater();
}

float CWaterLevelEx::CalcDistanceToWater(float fX, float fY)
{
#if 0
	if ( false )
	{
		if (apWaterLevels[WATER_ORIGINAL]->CalcDistanceToWater(fX, fY)
			!= apWaterLevels[WATER_FH]->CalcDistanceToWater(fX, fY))
			printf("CalcDistanceToWater fucked up\n");
	}
#endif	
	return Get()->CalcDistanceToWater(fX, fY);
}

void CWaterLevelEx::PreCalcWaterGeometry(void)
{
	Get()->PreCalcWaterGeometry();
}

void CWaterLevelEx::NoSeaBedCheat()
{
	for ( Int32 i = 0; i < WATER_MAX; i++ )
		apWaterLevels[i]->NoSeaBedCheat();
}

void CWaterLevelEx::RenderWaterLayersCheat()
{
	for ( Int32 i = 0; i < WATER_MAX; i++ )
		apWaterLevels[i]->RenderWaterLayersCheat();
}
	
void CWaterLevelEx::AllocateBoatWakeArray()
{
	for ( Int32 i = 0; i < WATER_MAX; i++ )
		apWaterLevels[i]->AllocateBoatWakeArray();
}

void CWaterLevelEx::FreeBoatWakeArray()
{
	for ( Int32 i = 0; i < WATER_MAX; i++ )
		apWaterLevels[i]->FreeBoatWakeArray();
}