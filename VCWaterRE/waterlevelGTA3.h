#pragma once
#include "_precl.h"
#include "_config.h"

namespace FHGTA3
{
#define WATER_BLOCK_SIZE 64
#define WATER_FINEBLOCK_SIZE 128

#define WATER_Z_OFFSET (0.5f)

#define MAX_SMALL_SECTORS      128
#define MAX_LARGE_SECTORS      64
#define MAX_HUGE_SECTORS       32
#define MAX_EXTRAHUGE_SECTORS  16

#define SMALL_SECTOR_SIZE      32
#define LARGE_SECTOR_SIZE      64
#define HUGE_SECTOR_SIZE       128
#define EXTRAHUGE_SECTOR_SIZE  256

#define WATER_START_X -2048.0f
#define WATER_END_X    2048.0f

#define WATER_START_Y -2048.0f
#define WATER_END_Y    2048.0f

#define WATER_WIDTH		((WATER_END_X - WATER_START_X))
#define WATER_HEIGHT	((WATER_END_Y - WATER_START_Y))


#define WATER_UNSIGN_X(x)					( (x) + (WATER_WIDTH /2) )
#define WATER_UNSIGN_Y(y)					( (y) + (WATER_HEIGHT/2) )
#define WATER_SIGN_X(x)						( (x) - (WATER_WIDTH /2) )
#define WATER_SIGN_Y(y)						( (y) - (WATER_HEIGHT/2) )

// 32
#define WATER_SMALL_X(x)					( WATER_UNSIGN_X(x)					/ MAX_SMALL_SECTORS     )
#define WATER_SMALL_Y(y)					( WATER_UNSIGN_Y(y)					/ MAX_SMALL_SECTORS     )
#define WATER_FROM_SMALL_SECTOR_X(x)		( ((x) - (MAX_SMALL_SECTORS/2)    ) * SMALL_SECTOR_SIZE     )
#define WATER_FROM_SMALL_SECTOR_Y(y)		( ((y) - (MAX_SMALL_SECTORS/2)    ) * SMALL_SECTOR_SIZE     )
#define WATER_TO_SMALL_SECTOR_X(x)			( WATER_UNSIGN_X(x)					/ SMALL_SECTOR_SIZE     )
#define WATER_TO_SMALL_SECTOR_Y(y)			( WATER_UNSIGN_Y(y)					/ SMALL_SECTOR_SIZE     )
			
// 64			
#define WATER_LARGE_X(x)					( WATER_UNSIGN_X(x)					/ MAX_LARGE_SECTORS     )
#define WATER_LARGE_Y(y)					( WATER_UNSIGN_Y(y)					/ MAX_LARGE_SECTORS     )
#define WATER_FROM_LARGE_SECTOR_X(x)		( ((x) - (MAX_LARGE_SECTORS/2)    ) * LARGE_SECTOR_SIZE     )
#define WATER_FROM_LARGE_SECTOR_Y(y)		( ((y) - (MAX_LARGE_SECTORS/2)    ) * LARGE_SECTOR_SIZE     )
#define WATER_TO_LARGE_SECTOR_X(x)			( WATER_UNSIGN_X(x)					/ LARGE_SECTOR_SIZE     )
#define WATER_TO_LARGE_SECTOR_Y(y)			( WATER_UNSIGN_Y(y)					/ LARGE_SECTOR_SIZE     )
				
// 128				
#define WATER_HUGE_X(x)						( WATER_UNSIGN_X(x)					/ MAX_HUGE_SECTORS      )
#define WATER_HUGE_Y(y)						( WATER_UNSIGN_Y(y)					/ MAX_HUGE_SECTORS      )
#define WATER_FROM_HUGE_SECTOR_X(x)			( ((x) - (MAX_HUGE_SECTORS/2)     ) * HUGE_SECTOR_SIZE      )
#define WATER_FROM_HUGE_SECTOR_Y(y)			( ((y) - (MAX_HUGE_SECTORS/2)     ) * HUGE_SECTOR_SIZE      )
#define WATER_TO_HUGE_SECTOR_X(x)			( WATER_UNSIGN_X(x)					/ HUGE_SECTOR_SIZE      )
#define WATER_TO_HUGE_SECTOR_Y(y)			( WATER_UNSIGN_Y(y)					/ HUGE_SECTOR_SIZE      )

// 256	
#define WATER_EXTRAHUGE_X(x)				( WATER_UNSIGN_X(x)					/ MAX_EXTRAHUGE_SECTORS )
#define WATER_EXTRAHUGE_Y(y)				( WATER_UNSIGN_Y(y)					/ MAX_EXTRAHUGE_SECTORS )
#define WATER_FROM_EXTRAHUGE_SECTOR_X(x)	( ((x) - (MAX_EXTRAHUGE_SECTORS/2)) * EXTRAHUGE_SECTOR_SIZE )
#define WATER_FROM_EXTRAHUGE_SECTOR_Y(y)	( ((y) - (MAX_EXTRAHUGE_SECTORS/2)) * EXTRAHUGE_SECTOR_SIZE )
#define WATER_TO_EXTRAHUGE_SECTOR_X(x)		( WATER_UNSIGN_X(x)					/ EXTRAHUGE_SECTOR_SIZE )
#define WATER_TO_EXTRAHUGE_SECTOR_Y(y)		( WATER_UNSIGN_Y(y)					/ EXTRAHUGE_SECTOR_SIZE )
/*
struct ColData
{
	UInt8 SurfaceType;
	UInt8 PieceType;
};
*/	

class CWaterLevel
{
public:
	static Int32 ms_nNoOfWaterLevels;
    
	static Float ms_aWaterZs[48];
	static CRect ms_aWaterRects[48];
	
	static UInt8 aWaterBlockList[WATER_BLOCK_SIZE][WATER_BLOCK_SIZE];
	static UInt8 aWaterFineBlockList[WATER_FINEBLOCK_SIZE][WATER_FINEBLOCK_SIZE];

	static Bool WavesCalculatedThisFrame;
	
	static Bool RequireWavySector;
	static Bool MaskCalculatedThisFrame;
	static CVector PreCalculatedMaskPosn;
	static Bool m_bRenderSeaBed;
	static Int32 m_nRenderWaterLayers;
	
	static RpAtomic *ms_pWavyAtomic;
	static RpAtomic *ms_pMaskAtomic;
	
	static RpGeometry *apGeomArray[8];
	static Int16 nGeomUsed;

	
	// III PS2
	static void Initialise(char *pWaterCfg);
	static void Shutdown();
    
	static void CreateWavyAtomic();
	// inlined
	static void DestroyWavyAtomic();

	static Bool GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ);
	static Bool GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel);
	static float GetWaterWavesOnly(short x, short y);	// unused
	static CVector GetWaterNormal(float fX, float fY);

	static void RenderWater();
	
	static void RenderTransparentWater(void)
	{ }

	static void RenderOneFlatSmallWaterPoly    (float fX, float fY, float fZ, RwRGBA const &color);
	static void RenderOneFlatLargeWaterPoly    (float fX, float fY, float fZ, RwRGBA const &color);	
	static void RenderOneFlatHugeWaterPoly     (float fX, float fY, float fZ, RwRGBA const &color);
	static void RenderOneFlatExtraHugeWaterPoly(float fX, float fY, float fZ, RwRGBA const &color);
	static void RenderOneWavySector            (float fX, float fY, float fZ, RwRGBA const &color, Bool bUnk = false);
	
	static void PreCalcWaterGeometry(void)
	{ }
	
	static void RenderBoatWakes(void);
	static void RenderWakeSegment(CVector2D &vecA, CVector2D &vecB, CVector2D &vecC, CVector2D &vecD, float &fSizeA, float &fSizeB, float &fAlphaA, float &fAlphaB, float &fWakeZ);
	
	// unused
	static float CalcDistanceToWater(float fX, float fY);
	static void RenderAndEmptyRenderBuffer();
	
	static Bool GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance);
	
	// unused
	static Bool IsLocationOutOfWorldBounds_WS(CVector const &vecPosn, int nOffset);
	// unused
	static Bool GetGroundLevel_WS(CVector const & vecPosn, float *pfOutLevel, ColData *pData, float fDistance);
	static Bool GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel);
	
	static void RenderSeaBirds();
	static void RenderShipsOnHorizon();
	
	static void HandleSeaLifeForms();
	
	static void HandleBeachToysStuff(void);
	
	static void AllocateBoatWakeArray();
	static void FreeBoatWakeArray();
	
	//CWaterLevel::CreateBeachToy(CVector const&,eBeachToy)	
};

//IsOnScreen(CVector const&,float,bool)	

extern void WaterLevelInitialise(char *datFile);
}