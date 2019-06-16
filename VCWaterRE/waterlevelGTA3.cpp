#include "waterlevelGTA3.h"
#include "tweak.h"
#include "utils.h"
#include "Errors.h"
#include "settings.h"

namespace FHGTA3
{

Float TEXTURE_ADDU;
Float TEXTURE_ADDV;

Int32 CWaterLevel::ms_nNoOfWaterLevels;

Float CWaterLevel::ms_aWaterZs[48];
CRect CWaterLevel::ms_aWaterRects[48];

UInt8 CWaterLevel::aWaterBlockList[WATER_BLOCK_SIZE][WATER_BLOCK_SIZE];
UInt8 CWaterLevel::aWaterFineBlockList[WATER_FINEBLOCK_SIZE][WATER_FINEBLOCK_SIZE];

Bool CWaterLevel::WavesCalculatedThisFrame;


Bool CWaterLevel::RequireWavySector;
Bool CWaterLevel::MaskCalculatedThisFrame;
CVector CWaterLevel::PreCalculatedMaskPosn;
Bool CWaterLevel::m_bRenderSeaBed;
Int32 CWaterLevel::m_nRenderWaterLayers;

RpAtomic *CWaterLevel::ms_pWavyAtomic;
RpAtomic *CWaterLevel::ms_pMaskAtomic;

RpGeometry *CWaterLevel::apGeomArray[8];
Int16 CWaterLevel::nGeomUsed;


RwTexture *gpWaterTex;
RwTexture *gpWaterEnvTex;
RwTexture *gpWaterEnvBaseTex;
RwTexture *gpWaterWakeTex;

RwRaster *gpWaterRaster;
RwRaster *gpWaterEnvRaster;
RwRaster *gpWaterEnvBaseRaster;
RwRaster *gpWaterWakeRaster;

Bool _bSeaLife;
Float _fWaterZOffset = 1.5;


Int32 TempBufferVerticesStored;
Int32 TempBufferIndicesStored;

#define TEMPBUFFERVERTSIZE 512
#define TEMPBUFFERINDEXSIZE 1024

RwImVertexIndex TempBufferRenderIndexList[TEMPBUFFERINDEXSIZE];
RwIm3DVertex TempBufferRenderVertices[TEMPBUFFERVERTSIZE];

#define RwIm3DVertexSet_RGBA(vert, rgba) RwIm3DVertexSetRGBA(vert, rgba.red, rgba.green, rgba.blue, rgba.alpha) // (RwRGBAAssign(&(_dst)->color, &_src))

//
/*
const Float fAdd1 = 180.0f;
const Float fAdd2 = 80.0f;
const Float fRedMult = 0.6f;
const Float fGreenMult = 1.0f;
const Float fBlueMult = 1.4f;
const Float MAX_WAKE_LENGTH = 50.0f;
const Float MIN_WAKE_INTERVAL = 1.0f;
const Float WAKE_LIFETIME = 400.0f;
*/

//SETTWEAKPATH("GTA3");
//TWEAKFLOAT(fEnvScale,               0.5f, 0.0f,     1.0f,    0.01f);	//	CWaterLevel::CreateWavyAtomic
//TWEAKFLOAT(fWave2InvLength,         0.03f, 0.0f,     1.0f,    0.01f);	//	CWaterLevel::PreCalcWavySector
//TWEAKFLOAT(fWave2NormScale,         0.5f,  0.0f,     1.0f,    0.01f);	//	CWaterLevel::PreCalcWavySector
//TWEAKFLOAT(fWave2Ampl,              0.1f,  0.0f,     1.0f,    0.01f);	//	CWaterLevel::PreCalcWavySector
//UInt8 nWaterAlpha             = 192;
//UInt8 nWakeAlpha              = 192;
//float fUnder1                 = 4.0;
//float fUnder2                 = 2.5;
//float fUnder3                 = 1.5;
//int nMaskAlpha                = 230;
float fAdd1                   = 180.0;
float fAdd2                   = 80.0;
float fRedMult                = 0.6;
float fGreenMult              = 1.0;
float fBlueMult               = 1.4;
float fAlphaMult              = 500.0;
float fAlphaBase              = 30.0;
//TWEAKFLOAT(fRandomMoveDiv,          8.0f,   0.0f,     100.0f, 1.0f);	//	CWaterLevel::PreCalcWavySector
//TWEAKFLOAT(fRandomDamp,             0.99f,  0.0f,     100.0f, 0.01f);	//	CWaterLevel::PreCalcWavySector
//TWEAKFLOAT(fNormMult,               2.0f,   0.0f,     100.0f, 0.01f);	//	CWaterLevel::PreCalcWavySector
//TWEAKFLOAT(fNormMultB,              1.0f,   0.0f,     100.0f, 0.01f);	//	CWaterLevel::PreCalcWavySector
//float fBumpScale              = 1.5;
//float fBumpTexRepeat          = 2.0;
//TWEAKFLOAT(fNormalDirectionScalar1, 2.0f,   0.0f,     100.0f, 0.1f);	//	CWaterLevel::PreCalcWavySector
//TWEAKFLOAT(fNormalDirectionScalar2, 1.0f,   0.0f,     100.0f, 0.1f);	//	CWaterLevel::PreCalcWavySector
//Bool bTestDoNormals           = true;
//TWEAKFLOAT(fSeaBedZ,                25.0f,  0.0f,     100.0f, 1.0f);	//	CWaterLevel::RenderOneSlopedUnderWaterPoly
///float aAlphaFade[5]               = { 0.4f, 1.0f, 0.2f, 1.0f, 0.4f};	//CWaterLevel::RenderWakeSegment
//TWEAKFLOAT(fFlatWaterBlendRange,    0.05f,  0.0f,     1.0f,   0.01f);	//	CWaterLevel::RenderOneFlatSmallWaterPolyBlended
//TWEAKFLOAT(fStartBlendDistanceAdd,  64.0f,  0.0f,     100.0f, 1.0f);	//	CWaterLevel::RenderOneFlatSmallWaterPolyBlended
//TWEAKFLOAT(fMinWaterAlphaMult,      -30.0f, -100.0f,  100.0f, 1.0f);	//	CWaterLevel::RenderOneFlatSmallWaterPolyBlended


Float fShapeLength = 0.4f;
Float fShapeTime = 0.05f;
Float fRangeMult = 0.6f; // 0.75f gta 3

Bool IsSectorAffectedByWake(CVector2D sector, Float fSize, CBoat **apBoats)
{
	Bool numVerts = 0;
	
	if ( CBoat::apFrameWakeGeneratingBoats[0] == NULL )
		return false;
	
	for ( Int32 i = 0; i < 4; i++ )
	{
		CBoat *pBoat = CBoat::apFrameWakeGeneratingBoats[i];
		if ( !pBoat )
			break;
		
		for ( int j = 0; j < pBoat->m_nNumWaterTrailPoints; j++ )
		{
			 Float fDist = (CBoat::WAKE_LIFETIME - pBoat->m_afWakePointLifeTime[j]) * fShapeTime + Float(j) * fShapeLength + fSize;
			 
			 if ( fabs(pBoat->m_avecWakePoints[j].x - sector.x) < fDist
				&& fabs(pBoat->m_avecWakePoints[i].y - sector.y) < fDist )
			 {
				 apBoats[numVerts] = pBoat;
				 numVerts = 1; // += ?
				 break;
			 }
		}
	}
	
	return numVerts != 0;
}

Float IsVertexAffectedByWake(CVector vecVertex, CBoat *pBoat)
{
	Float &fTimeMult = *(Float*)0xA0FCF4;
	for ( int i = 0; i < pBoat->m_nNumWaterTrailPoints; i++ )
	{
		Float fMaxDist = (CBoat::WAKE_LIFETIME - pBoat->m_afWakePointLifeTime[i]) * fShapeTime + Float(i) * fShapeLength;
		
		Float fX = pBoat->m_avecWakePoints[i].x - vecVertex.x;
		Float fY = pBoat->m_avecWakePoints[i].y - vecVertex.y;
		
		Float fDist = fY * fY + fX * fX;
		
		if ( fDist < SQR(fMaxDist) )
			return 1.0f - min(fRangeMult * sqrt(fDist / SQR(fMaxDist)) + (CBoat::WAKE_LIFETIME - pBoat->m_afWakePointLifeTime[i]) * fTimeMult, 1.0f);
	}

	return 0.0f;
}

void WaterLevelInitialise(char *datFile)
{	
	CWaterLevel::ms_nNoOfWaterLevels = 0;
	
	Handle hFile;
	
	do
	{
		hFile = CFileMgr::OpenFile("DATA\\waterpro.dat", "rb");
	}
	while ( !SUCCEEDED(hFile) );
	
	if ( hFile > 0 )
	{
		if ( SUCCEEDED(hFile) )
		{
			CFileMgr::Read(hFile, (char *)&CWaterLevel::ms_nNoOfWaterLevels, sizeof(CWaterLevel::ms_nNoOfWaterLevels));
			CFileMgr::Read(hFile, (char *)CWaterLevel::ms_aWaterZs,	sizeof(CWaterLevel::ms_aWaterZs));
			CFileMgr::Read(hFile, (char *)CWaterLevel::ms_aWaterRects, sizeof(CWaterLevel::ms_aWaterRects));
			CFileMgr::Read(hFile, (char *)CWaterLevel::aWaterBlockList, sizeof(CWaterLevel::aWaterBlockList));
			CFileMgr::Read(hFile, (char *)CWaterLevel::aWaterFineBlockList, sizeof(CWaterLevel::aWaterFineBlockList));
		}
		
		CFileMgr::CloseFile(hFile);
	}
	
	CTxdStore::PushCurrentTxd();

#if 0	
	Int32 slot = CTxdStore::FindTxdSlot("particle");
#else
	Int32 slot = CTxdStore::AddTxdSlot("waterfhg3");
	
	Char Path[MAX_PATH];
	sprintf(Path,"%s\\WaterEx\\GTA3\\WATER.TXD", GetAsiPath().c_str());
	
	if ( !file_exists(Path) )
		FATAL("Can't Open: %s", Path);

	CTxdStore::LoadTxd(slot, Path);
	
	CTxdStore::AddRef(slot);
#endif
	
	CTxdStore::SetCurrentTxd(slot);
	
	
	if ( gpWaterTex == NULL )
		gpWaterTex = RwTextureRead("water_old", NULL);
	gpWaterRaster = RwTextureGetRaster(gpWaterTex);
	
	CTxdStore::PopCurrentTxd();

	CWaterLevel::CreateWavyAtomic();
	CWaterLevel::FreeBoatWakeArray();
	
	printf("Done Initing waterlevels\n");
}

void CWaterLevel::Initialise(char *pWaterCfg)
{
	WaterLevelInitialise(pWaterCfg);
}

void CWaterLevel::Shutdown()
{
	FreeBoatWakeArray(); // TODO:
	DestroyWavyAtomic();

#define _DELETE_TEXTURE(t) if ( t )	\
		{ \
			RwTextureDestroy(t); \
			t = NULL; \
		}

	_DELETE_TEXTURE(gpWaterTex);

#undef _DELETE_TEXTURE

#if 0
	//
#else
	Int32 slot;
	slot = CTxdStore::FindTxdSlot("waterfhg3");
	CTxdStore::RemoveTxdSlot(slot);
#endif
}

void CWaterLevel::CreateWavyAtomic()
{
	RpGeometry *wavyGeometry;
	RpMaterial *wavyMaterial;
	RpTriangle *wavytlist;
	RpMorphTarget *wavyMorphTarget;
	RwSphere boundingSphere;	
	RwV3d *wavyVert;

	RwFrame *wavyFrame;
	
	{
		wavyGeometry = RpGeometryCreate(9*9, 128, rpGEOMETRYTRISTRIP
													|rpGEOMETRYTEXTURED
													|rpGEOMETRYPRELIT
													|rpGEOMETRYMODULATEMATERIALCOLOR);
	
	}
	
	{
		wavyMaterial = RpMaterialCreate();
		RpMaterialSetTexture(wavyMaterial, gpWaterTex);
	}
	
	{
		wavytlist = RpGeometryGetTriangles(wavyGeometry);
		
		for ( Int32 i = 0; i < 8; i++ )
		{
			for ( Int32 j = 0; j < 8; j++ )
			{
				const RwUInt16 base = (RwUInt16)((8 + 1)*i+j);
	
				RpGeometryTriangleSetVertexIndices(wavyGeometry, 
						wavytlist,     (RwInt16)base, (RwInt16)(base+1),    (RwInt16)(base+8+2));

				RpGeometryTriangleSetVertexIndices(wavyGeometry, 
						(wavytlist+1), (RwInt16)base, (RwInt16)(base+8+2), (RwInt16)(base+8+1));
	
				RpGeometryTriangleSetMaterial(wavyGeometry, wavytlist,     wavyMaterial);
				RpGeometryTriangleSetMaterial(wavyGeometry, (wavytlist+1), wavyMaterial);
				
				wavytlist+=2;
			}
		}
	}
	

	{
		wavyMorphTarget = RpGeometryGetMorphTarget(wavyGeometry, 0);
		wavyVert = RpMorphTargetGetVertices(wavyMorphTarget);
		
		for ( Int32 i = 0; i < 9; i++ )
		{
			for ( Int32 j = 0; j < 9; j++ )
			{
				(*wavyVert).x = (Float)i * 4.0f;
				(*wavyVert).y = (Float)j * 4.0f;
				(*wavyVert).z = 0.0f;
				
				wavyVert++;
			}
		}
		
		RpMorphTargetCalcBoundingSphere(wavyMorphTarget, &boundingSphere);
		RpMorphTargetSetBoundingSphere(wavyMorphTarget, &boundingSphere);
		RpGeometryUnlock(wavyGeometry);
	}
	
	
	{
		wavyFrame = RwFrameCreate();
		ms_pWavyAtomic = RpAtomicCreate();
		RpAtomicSetGeometry(ms_pWavyAtomic, wavyGeometry, 0);
		RpAtomicSetFrame(ms_pWavyAtomic, wavyFrame);
		RpMaterialDestroy(wavyMaterial);
		RpGeometryDestroy(wavyGeometry);
	}
}

void CWaterLevel::DestroyWavyAtomic()
{
#define _DELETE_ATOMIC(a) \
		{ \
			RwFrame *frame; \
			frame = RpAtomicGetFrame(a); \
			RpAtomicDestroy(a); \
			RwFrameDestroy(frame); \
		}

	_DELETE_ATOMIC(ms_pWavyAtomic);
	
#undef _DELETE_ATOMIC
}

Bool CWaterLevel::GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ)
{
	Int32 x = WATER_HUGE_X(fX + 400.0f);
	Int32 y = WATER_HUGE_Y(fY);
	
	if ( x < 0 || x >= 128 )
		return false;
	
	if ( y < 0 || y >= 128 )
		return false;

	UInt8 nBlock = aWaterFineBlockList[x][y];

	if ( nBlock == 128 )
		return false;

	*pfOutLevel = ms_aWaterZs[nBlock];

	//	
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	
	Float fWave = sin
	(
		( WATER_UNSIGN_Y(fY)         - Float(y) * MAX_HUGE_SECTORS
		+ WATER_UNSIGN_X(fX + 400.0) - Float(x) * MAX_HUGE_SECTORS )
		
		* (_2PI / MAX_HUGE_SECTORS ) + fAngle
	);
		
	//Float fWindFactor = CWeather::WindClipped * 0.4f + 0.2f;
	Float fWindFactor = CWeather::Wind * 0.7f + 0.3f;
	
	*pfOutLevel += fWave * fWindFactor;

	if ( bDontCheckZ == false && (*pfOutLevel - fZ) > 3.0f )
	{
		*pfOutLevel = 0.0f;
		return false;
	}

	return true;
}

Bool CWaterLevel::GetWaterLevelNoWaves(float fX, float fY, float fZ, float *pfOutLevel)
{
	Int32 x = WATER_HUGE_X(fX + 400.0f);
	Int32 y = WATER_HUGE_Y(fY);
	
	UInt8 nBlock = aWaterFineBlockList[x][y];
		
	if ( nBlock == 128 )
		return false;
	
	*pfOutLevel = ms_aWaterZs[nBlock];

	return true;
}

float CWaterLevel::GetWaterWavesOnly(short x, short y)
{
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	
	//Float fWindFactor = CWeather::WindClipped * 0.7f + 0.3f;	//gta3, haha
	Float fWindFactor = CWeather::Wind * 0.7f + 0.3f;	//gta3, haha
	
	Float fWave = sinf( Float(Float(4 * y + 4 * x) * (_2PI / MAX_HUGE_SECTORS )) + fAngle );
	
	return fWave * fWindFactor;
}
	   
CVector CWaterLevel::GetWaterNormal(float fX, float fY)
{
	Int32 x = WATER_HUGE_X(fX);
	Int32 y = WATER_HUGE_Y(fY);
	
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	//Float fWindFactor = CWeather::WindClipped * 0.4f + 0.2f;
	Float fWindFactor = CWeather::Wind * 0.7f + 0.3f;
	
	//TODO: 
	Float _fVAngle = ((2048.0f + fY) - y * (Float)MAX_HUGE_SECTORS
					+ (2048.0f + fX) - x * (Float)MAX_HUGE_SECTORS )
					* (_2PI / MAX_HUGE_SECTORS ) + fAngle;

	CVector vA(1.0f, 0.0f, fWindFactor * (_2PI / MAX_HUGE_SECTORS ) * cos(_fVAngle));
	CVector vB(0.0f, 1.0f, fWindFactor * (_2PI / MAX_HUGE_SECTORS ) * cos(_fVAngle));
	
	CVector norm = CrossProduct(vA, vB);
	
	norm.Normalise();
	
	return norm;
}
/*
Float _GetWaterDrawDist()
{
	if ( TheCamera.m_sCoords.pos.z < 15.0f )
		return 1200.0f;
	else
	{
		if ( TheCamera.m_sCoords.pos.z > 60.0f )
			return 2000.0f;
		else
			return (TheCamera.m_sCoords.pos.z + -15.0f) * 800.0f / 45.0f + 1200.0f;
	}
}
*/

inline Float _GetWaterDrawDist()
{
	// if z less then 15.0f return 1200.0f 
	if ( TheCamera.m_sCoords.pos.z < 15.0f )
		return 1200.0f;

	// if z greater then 60.0f return 2000.0f;
	if ( TheCamera.m_sCoords.pos.z > 60.0f )
		return 2000.0f;

	return (TheCamera.m_sCoords.pos.z + -15.0f) * 800.0f / 45.0f + 1200.0f;
}

inline Float _GetTransparentWaterDrawDist()
{
	if ( FindPlayerVehicle() && FindPlayerVehicle()->m_nVehicleClass == VEHICLETYPE_BOAT )
		return fTransparentFarDistInBoat; //return 120.0f;
	else
		return fTransparentFarDist; //return 70.0f;
}

inline void _GetCamBounds(Bool *bUseCamStartY, Bool *bUseCamEndY, Bool *bUseCamStartX, Bool *bUseCamEndX)
{
	if ( TheCamera.m_sCoords.up.z > -0.8f )
	{
		if ( fabsf(TheCamera.m_sCoords.up.x) > fabsf(TheCamera.m_sCoords.up.y) )
		{
			if ( TheCamera.m_sCoords.up.x > 0.0f )
				*bUseCamStartX = true;
			else
				*bUseCamEndX = true;
		}
		else
		{
			if ( TheCamera.m_sCoords.up.y > 0.0f )
				*bUseCamStartY = true;
			else
				*bUseCamEndY = true;
		}
	}
}

Float SectorRadius(Float fSize)
{
	return sqrtf(powf(fSize, 2) + powf(fSize, 2)) * 1.0040916293f;
}

void CWaterLevel::RenderWater()
{
	Bool bUseCamEndX   = false;
	Bool bUseCamStartY = false;
	
	Bool bUseCamStartX = false;
	Bool bUseCamEndY   = false;
	
	if ( !CGame::CanSeeWaterFromCurrArea() )
		return;
	
	_GetCamBounds(&bUseCamStartY, &bUseCamEndY, &bUseCamStartX, &bUseCamEndX);

	Float fHugeSectorMaxRenderDist    = _GetWaterDrawDist();
	Float fHugeSectorMaxRenderDistSqr = SQR(fHugeSectorMaxRenderDist);
	
	Float fWaterDrawDist      = _GetTransparentWaterDrawDist();
	Float fWaterDrawDistLarge = fWaterDrawDist + 90.0f;
	Float fWavySectorMaxRenderDistSqr   = SQR(fWaterDrawDist);
	
	Float windAddUV = CWeather::Wind * 0.0015f + 0.0005f;
	
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	
	if ( !CTimer::m_UserPause && !CTimer::m_CodePause )
	{
		TEXTURE_ADDU += CGeneral::GetRandomNumberInRange(-0.0005f, 0.0005f) + windAddUV;
		TEXTURE_ADDV += CGeneral::GetRandomNumberInRange(-0.0005f, 0.0005f) + windAddUV;
	}
	
	if ( TEXTURE_ADDU >= 1.0f )
		TEXTURE_ADDU = 0.0f;
	if ( TEXTURE_ADDV >= 1.0f )
		TEXTURE_ADDV = 0.0f;
	
	WavesCalculatedThisFrame = false;

	RwRGBA color = { 0, 0, 0, 255 };
	
	color.red   = UInt32((CTimeCycle::m_fCurrentDirectionalRed   * 0.5f + CTimeCycle::m_fCurrentAmbientRed)   * 255.0f);
	color.green = UInt32((CTimeCycle::m_fCurrentDirectionalGreen * 0.5f + CTimeCycle::m_fCurrentAmbientGreen) * 255.0f);
	color.blue  = UInt32((CTimeCycle::m_fCurrentDirectionalBlue  * 0.5f + CTimeCycle::m_fCurrentAmbientBlue)  * 255.0f);

	TempBufferVerticesStored = 0;
	TempBufferIndicesStored = 0;
	
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)gpWaterRaster);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE,     (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND,      (void *)rwBLENDONE);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND,     (void *)rwBLENDZERO);
	
	CVector2D camPos
	(
		TheCamera.m_sCoords.pos.x,
		TheCamera.m_sCoords.pos.y
	);

	Int32 nStartX = WATER_TO_HUGE_SECTOR_X(camPos.x - fHugeSectorMaxRenderDist + 400.0f);
	Int32 nEndX   = WATER_TO_HUGE_SECTOR_X(camPos.x + fHugeSectorMaxRenderDist + 400.0f) + 1;
	Int32 nStartY = WATER_TO_HUGE_SECTOR_Y(camPos.y - fHugeSectorMaxRenderDist         );
	Int32 nEndY   = WATER_TO_HUGE_SECTOR_Y(camPos.y + fHugeSectorMaxRenderDist         ) + 1;

	if ( bUseCamStartX )
		nStartX = WATER_TO_HUGE_SECTOR_X(camPos.x + 400.0f);
	if ( bUseCamEndX )
		nEndX   = WATER_TO_HUGE_SECTOR_X(camPos.x + 400.0f);
	if ( bUseCamStartY )
		nStartY = WATER_TO_HUGE_SECTOR_Y(camPos.y         );
	if ( bUseCamEndY )                                      
		nEndY   = WATER_TO_HUGE_SECTOR_Y(camPos.y         );

	nStartX = clamp(nStartX, 0, MAX_HUGE_SECTORS - 1);
	nEndX   = clamp(nEndX,   0, MAX_HUGE_SECTORS - 1);
	nStartY = clamp(nStartY, 0, MAX_HUGE_SECTORS - 1);
	nEndY   = clamp(nEndY,   0, MAX_HUGE_SECTORS - 1);
	
	for ( Int32 x = nStartX; x <= nEndX; x++ )
	{
		for ( Int32 y = nStartY; y <= nEndY; y++ )
		{
			if (   !(aWaterBlockList[2*x+0][2*y+0] & 0x80)
				|| !(aWaterBlockList[2*x+1][2*y+0] & 0x80)
				|| !(aWaterBlockList[2*x+0][2*y+1] & 0x80)
				|| !(aWaterBlockList[2*x+1][2*y+1] & 0x80) )
			{
				Float fX = WATER_FROM_HUGE_SECTOR_X(x) - 400.0f;
				Float fY = WATER_FROM_HUGE_SECTOR_Y(y);

				CVector2D vecHugeSectorCentre
				(
					fX + HUGE_SECTOR_SIZE/2,
					fY + HUGE_SECTOR_SIZE/2
				);

				Float fHugeSectorDistToCamSqr = (camPos - vecHugeSectorCentre).MagnitudeSqr();
				/////////////////////////////////////////////////////////////
				if ( fHugeSectorMaxRenderDistSqr > fHugeSectorDistToCamSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecHugeSectorCentre), SectorRadius(HUGE_SECTOR_SIZE), //181.75999f,
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						if ( fHugeSectorDistToCamSqr >= SQR(fHugeSectorNearDist) /*SQR(500.0f)*/ )
						{
							Float fZ;
	
							if ( !(aWaterBlockList[2*x+0][2*y+0] & 0x80) )
								fZ = ms_aWaterZs[ aWaterBlockList[2*x+0][2*y+0] ];
	
							if ( !(aWaterBlockList[2*x+1][2*y+0] & 0x80) )
								fZ = ms_aWaterZs[ aWaterBlockList[2*x+1][2*y+0] ];
	
							if ( !(aWaterBlockList[2*x+0][2*y+1] & 0x80) )
								fZ = ms_aWaterZs[ aWaterBlockList[2*x+0][2*y+1] ];
	
							if ( !(aWaterBlockList[2*x+1][2*y+1] & 0x80) )
								fZ = ms_aWaterZs[ aWaterBlockList[2*x+1][2*y+1] ];
							
							RenderOneFlatHugeWaterPoly(fX, fY, fZ, color);
						}
						else
						{
							for ( Int32 x2 = 2*x; x2 <= 2*x+1; x2++ )
							{
								for ( Int32 y2 = 2*y; y2 <= 2*y+1; y2++ )
								{
									if ( !(aWaterBlockList[x2][y2] & 0x80) )
									{
										Float fLargeX = WATER_FROM_LARGE_SECTOR_X(x2) - 400.0f;
										Float fLargeY = WATER_FROM_LARGE_SECTOR_Y(y2);
						
										CVector2D vecLargeSectorCentre
										(
											fLargeX + LARGE_SECTOR_SIZE/2,
											fLargeY + LARGE_SECTOR_SIZE/2
										);
										
										Float fLargeSectorDistToCamSqr = (camPos - vecLargeSectorCentre).MagnitudeSqr();
										
										if ( fLargeSectorDistToCamSqr < fHugeSectorMaxRenderDistSqr )
										{
											if ( TheCamera.IsSphereVisible(CVector(vecLargeSectorCentre), SectorRadius(LARGE_SECTOR_SIZE), //90.879997f,
												&CMatrix(TheCamera.GetInverseMatrix())) )
											{
												// Render four small(32x32) sectors, or one large(64x64).

												//														
												//	       [N]
												//	    ---------
												//	    |0x1|1x1|
												//	[W] --------- [E]
												//	    |0x0|1x0|
												//	    ---------
												//	       [S]
												//	
												
												Float fLargeSectorDrawDistSqr = SQR((fWaterDrawDistLarge + 16.0f));
												
												if ( fLargeSectorDistToCamSqr < fLargeSectorDrawDistSqr )
												{
													//_bSeaLife = true;
													
													Float fZ;
													
													// WS
													if ( !(aWaterFineBlockList[2*x2+0][2*y2+0] & 0x80) )
													{
														Float fSmallX = fLargeX;
														Float fSmallY = fLargeY;
														
														CVector2D vecSmallSectorCentre
														(
															fSmallX + SMALL_SECTOR_SIZE/2,
															fSmallY + SMALL_SECTOR_SIZE/2
														);
														
														Float fSmallSectorDistToCamSqr = (camPos - vecSmallSectorCentre).MagnitudeSqr();
														fZ = ms_aWaterZs[ aWaterFineBlockList[2*x2+0][2*y2+0] ];
														
														if ( fSmallSectorDistToCamSqr < fWavySectorMaxRenderDistSqr )
															RenderOneWavySector(fSmallX, fSmallY, fZ, color);
														else
															RenderOneFlatSmallWaterPoly(fSmallX, fSmallY, fZ, color);
													}
													
													// SE
													if ( !(aWaterFineBlockList[2*x2+1][2*y2+0] & 0x80) )
													{
														Float fSmallX = fLargeX + (LARGE_SECTOR_SIZE/2);
														Float fSmallY = fLargeY;
														
														CVector2D vecSmallSectorCentre
														(
															fSmallX + SMALL_SECTOR_SIZE/2,
															fSmallY + SMALL_SECTOR_SIZE/2
														);
														
														Float fSmallSectorDistToCamSqr = (camPos - vecSmallSectorCentre).MagnitudeSqr();
														fZ = ms_aWaterZs[ aWaterFineBlockList[2*x2+1][2*y2+0] ];
														
														if ( fSmallSectorDistToCamSqr < fWavySectorMaxRenderDistSqr )
															RenderOneWavySector(fSmallX, fSmallY, fZ, color);
														else
															RenderOneFlatSmallWaterPoly(fSmallX, fSmallY, fZ, color);
													}
													
													// WN
													if ( !(aWaterFineBlockList[2*x2+0][2*y2+1] & 0x80) )
													{
														Float fSmallX = fLargeX;
														Float fSmallY = fLargeY + (LARGE_SECTOR_SIZE/2);
														
														CVector2D vecSmallSectorCentre
														(
															fSmallX + SMALL_SECTOR_SIZE/2,
															fSmallY + SMALL_SECTOR_SIZE/2
														);
														
														Float fSmallSectorDistToCamSqr = (camPos - vecSmallSectorCentre).MagnitudeSqr();
														fZ = ms_aWaterZs[ aWaterFineBlockList[2*x2+0][2*y2+1] ];
														
														if ( fSmallSectorDistToCamSqr < fWavySectorMaxRenderDistSqr )
															RenderOneWavySector(fSmallX, fSmallY, fZ, color);
														else
															RenderOneFlatSmallWaterPoly(fSmallX, fSmallY, fZ, color);
													}
													
													//NE
													if ( !(aWaterFineBlockList[2*x2+1][2*y2+1] & 0x80) )
													{
														Float fSmallX = fLargeX + (LARGE_SECTOR_SIZE/2);
														Float fSmallY = fLargeY + (LARGE_SECTOR_SIZE/2);
														
														CVector2D vecSmallSectorCentre
														(
															fSmallX + SMALL_SECTOR_SIZE/2,
															fSmallY + SMALL_SECTOR_SIZE/2
														);
														
														Float fSmallSectorDistToCamSqr = (camPos - vecSmallSectorCentre).MagnitudeSqr();
														fZ = ms_aWaterZs[ aWaterFineBlockList[2*x2+1][2*y2+1] ];
														
														if ( fSmallSectorDistToCamSqr < fWavySectorMaxRenderDistSqr )
															RenderOneWavySector(fSmallX, fSmallY, fZ, color);
														else
															RenderOneFlatSmallWaterPoly(fSmallX, fSmallY, fZ, color);
													}
												}
												else
												{
													Float fZ;
                                                
													fZ = ms_aWaterZs[ aWaterBlockList[x2][y2] ];
                                                
													RenderOneFlatLargeWaterPoly(fLargeX, fLargeY, fZ, color);
												}
											}	//	if ( TheCamera.IsSphereVisible
										}	//	if ( fLargeSectorDistToCamSqr < fHugeSectorMaxRenderDistSqr )
									}	//	if ( !(aWaterBlockList[x2][y2] & 0x80) )
								}	//	for ( Int32 y2 = 2*y; y2 <= 2*y+1; y2++ )
							}	//	for ( Int32 x2 = 2*x; x2 <= 2*x+1; x2++ )
							//
							
						}
					}
				}
			}
		}
	}
	
	/*
	 -----------      ----------------------       ----------------------
	|    [N]    |    |       [ EndY ]       |     |       [  top ]       |
	|           |    |                      |     |                      |
	|[W] [0] [E]|    |[StartX]  []  [ EndX ]|     |[ left ]  []  [ right]|
	|           |    |                      |     |                      |
	|    [S]    |    |       [StartY]       |     |       [bottom]       |
	 -----------      ----------------------       ----------------------
	
	
	[S] [StartY]  [bottom]
	[N] [EndY]    [top]
	[W] [StartX]  [left]
	[E] [EndX]    [right]
	
	[S]    -> [N] && [W]  -> [E]
	bottom -> top && left -> right
	*/

	for ( Int32 x = 0; x < 26; x++ )
	{
		for ( Int32 y = 0; y < 5; y++ )
		{
			Float fX = WATER_SIGN_X(Float(x) * EXTRAHUGE_SECTOR_SIZE) - 1280.0f - 400.0f;
			Float fY = WATER_SIGN_Y(Float(y) * EXTRAHUGE_SECTOR_SIZE) - 1280.0f;
			
			if ( !bUseCamStartY )
			{
				CVector2D vecExtraHugeSectorCentre
				(
					fX + EXTRAHUGE_SECTOR_SIZE/2,
					fY + EXTRAHUGE_SECTOR_SIZE/2
				);
				
				Float fCamDistToSector = (vecExtraHugeSectorCentre - camPos).Magnitude();
				
				if ( fCamDistToSector < fHugeSectorMaxRenderDistSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecExtraHugeSectorCentre), SectorRadius(EXTRAHUGE_SECTOR_SIZE), //363.51999f,
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						RenderOneFlatExtraHugeWaterPoly(
							vecExtraHugeSectorCentre.x - EXTRAHUGE_SECTOR_SIZE/2,
							vecExtraHugeSectorCentre.y - EXTRAHUGE_SECTOR_SIZE/2,
							0.0f,
							color);
					}
				}
			}
			
			if ( !bUseCamEndY )
			{
				CVector2D vecExtraHugeSectorCentre
				(
					fX + EXTRAHUGE_SECTOR_SIZE/2,
					-(fY + EXTRAHUGE_SECTOR_SIZE/2)
				);
				
				Float fCamDistToSector = (vecExtraHugeSectorCentre - camPos).Magnitude();
				
				if ( fCamDistToSector < fHugeSectorMaxRenderDistSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecExtraHugeSectorCentre), SectorRadius(EXTRAHUGE_SECTOR_SIZE), //363.51999f,
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						RenderOneFlatExtraHugeWaterPoly(
							vecExtraHugeSectorCentre.x - EXTRAHUGE_SECTOR_SIZE/2,
							vecExtraHugeSectorCentre.y - EXTRAHUGE_SECTOR_SIZE/2,
							0.0f,
							color);
					}
				}
			}

		}
	}
	
	for ( Int32 y = 5; y < 21; y++ )
	{
		for ( Int32 x = 0; x < 5; x++ )
		{
			Float fX = WATER_SIGN_X(Float(x) * EXTRAHUGE_SECTOR_SIZE) - 1280.0f - 400.0f;
			Float fX2 = WATER_SIGN_X(Float(x) * EXTRAHUGE_SECTOR_SIZE) - 1280.0f + 400.0f;
			Float fY = WATER_SIGN_Y(Float(y) * EXTRAHUGE_SECTOR_SIZE) - 1280.0f;
			
			if ( !bUseCamStartX )
			{
				CVector2D vecExtraHugeSectorCentre
				(
					fX + EXTRAHUGE_SECTOR_SIZE/2,
					fY + EXTRAHUGE_SECTOR_SIZE/2
				);
				
				Float fCamDistToSector = (vecExtraHugeSectorCentre - camPos).Magnitude();
				
				if ( fCamDistToSector < fHugeSectorMaxRenderDistSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecExtraHugeSectorCentre), SectorRadius(EXTRAHUGE_SECTOR_SIZE), //363.51999f,
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						RenderOneFlatExtraHugeWaterPoly(
							vecExtraHugeSectorCentre.x - EXTRAHUGE_SECTOR_SIZE/2,
							vecExtraHugeSectorCentre.y - EXTRAHUGE_SECTOR_SIZE/2,
							0.0f,
							color);
					}
				}
			}
			
			if ( !bUseCamEndX )
			{
				CVector2D vecExtraHugeSectorCentre
				(
					-(fX2 + EXTRAHUGE_SECTOR_SIZE/2),
					fY + EXTRAHUGE_SECTOR_SIZE/2
				);
				
				Float fCamDistToSector = (vecExtraHugeSectorCentre - camPos).Magnitude();
				
				if ( fCamDistToSector < fHugeSectorMaxRenderDistSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecExtraHugeSectorCentre), SectorRadius(EXTRAHUGE_SECTOR_SIZE), //363.51999f,
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						RenderOneFlatExtraHugeWaterPoly(
							vecExtraHugeSectorCentre.x - EXTRAHUGE_SECTOR_SIZE/2,
							vecExtraHugeSectorCentre.y - EXTRAHUGE_SECTOR_SIZE/2,
							0.0f,
							color);
					}
				}
			}
		}
	}

	RenderAndEmptyRenderBuffer();
	
	RenderSeaBirds();
/*
	//RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	//RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
	
	if ( WavesCalculatedThisFrame )
	{
		RenderSeaBirds();
		RenderShipsOnHorizon();
		CParticle::HandleShipsAtHorizonStuff();
		HandleBeachToysStuff();
	}
	
	if ( _bSeaLife )
		HandleSeaLifeForms();
*/
	DefinedState();
}

void CWaterLevel::RenderOneFlatSmallWaterPoly(float fX, float fY, float fZ, RwRGBA const &color)
{
	if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
		RenderAndEmptyRenderBuffer();
	
	Int32 vidx = TempBufferVerticesStored;
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 0], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + SMALL_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 1.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 1], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + SMALL_SECTOR_SIZE, fY + SMALL_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 1.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 1.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 2], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + SMALL_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 1.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 3], color);
	
	
	Int32 iidx = TempBufferIndicesStored;

	TempBufferRenderIndexList[iidx + 0] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 1] = TempBufferVerticesStored + 2;
	TempBufferRenderIndexList[iidx + 2] = TempBufferVerticesStored + 1;
	TempBufferRenderIndexList[iidx + 3] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 4] = TempBufferVerticesStored + 3;
	TempBufferRenderIndexList[iidx + 5] = TempBufferVerticesStored + 2;
	
	TempBufferVerticesStored += 4;

	TempBufferIndicesStored += 6;
}

void CWaterLevel::RenderOneFlatLargeWaterPoly(float fX, float fY, float fZ, RwRGBA const &color)
{
	if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
		RenderAndEmptyRenderBuffer();
	
	Int32 vidx = TempBufferVerticesStored;
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 0], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + LARGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 2.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 1], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + LARGE_SECTOR_SIZE, fY + LARGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 2.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 2.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 2], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + LARGE_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 2.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 3], color);


	Int32 iidx = TempBufferIndicesStored;

	TempBufferRenderIndexList[iidx + 0] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 1] = TempBufferVerticesStored + 2;
	TempBufferRenderIndexList[iidx + 2] = TempBufferVerticesStored + 1;
	TempBufferRenderIndexList[iidx + 3] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 4] = TempBufferVerticesStored + 3;
	TempBufferRenderIndexList[iidx + 5] = TempBufferVerticesStored + 2;
	
	TempBufferVerticesStored += 4;

	TempBufferIndicesStored += 6;
}

void CWaterLevel::RenderOneFlatHugeWaterPoly(float fX, float fY, float fZ, RwRGBA const &color)
{
	if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
		RenderAndEmptyRenderBuffer();
	
	Int32 vidx = TempBufferVerticesStored;

	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 0], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + HUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 4.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 1], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + HUGE_SECTOR_SIZE, fY + HUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 4.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 4.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 2], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + HUGE_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 4.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 3], color);


	Int32 iidx = TempBufferIndicesStored;

	TempBufferRenderIndexList[iidx + 0] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 1] = TempBufferVerticesStored + 2;
	TempBufferRenderIndexList[iidx + 2] = TempBufferVerticesStored + 1;
	TempBufferRenderIndexList[iidx + 3] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 4] = TempBufferVerticesStored + 3;
	TempBufferRenderIndexList[iidx + 5] = TempBufferVerticesStored + 2;
	
	TempBufferVerticesStored += 4;

	TempBufferIndicesStored += 6;
}

void CWaterLevel::RenderOneFlatExtraHugeWaterPoly(float fX, float fY, float fZ, RwRGBA const &color)
{
	if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
		RenderAndEmptyRenderBuffer();
	
	Int32 vidx = TempBufferVerticesStored;

	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 0], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + EXTRAHUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 8.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 1], color);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + EXTRAHUGE_SECTOR_SIZE, fY + EXTRAHUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 8.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 8.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 2], color);
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + EXTRAHUGE_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 8.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 3], color);


	Int32 iidx = TempBufferIndicesStored;

	TempBufferRenderIndexList[iidx + 0] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 1] = TempBufferVerticesStored + 2;
	TempBufferRenderIndexList[iidx + 2] = TempBufferVerticesStored + 1;
	TempBufferRenderIndexList[iidx + 3] = TempBufferVerticesStored + 0;
	TempBufferRenderIndexList[iidx + 4] = TempBufferVerticesStored + 3;
	TempBufferRenderIndexList[iidx + 5] = TempBufferVerticesStored + 2;
	
	TempBufferVerticesStored += 4;

	TempBufferIndicesStored += 6;
}

void CWaterLevel::RenderOneWavySector(float fX, float fY, float fZ, RwRGBA const &color, Bool bUnk)
{
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);	
	
	if ( !WavesCalculatedThisFrame )
	{
		nGeomUsed = 0;
		
		WavesCalculatedThisFrame = true;
		
		CBoat::FillBoatList();

		RpGeometry *geometry = RpAtomicGetGeometry(ms_pWavyAtomic);

		RwRGBA *prelights = RpGeometryGetPreLightColors(geometry);
		RwTexCoords *texCoord = RpGeometryGetVertexTexCoords(geometry, rwTEXTURECOORDINATEINDEX0);
		RwV3d *vlist = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(geometry, 0));

		RpGeometryLock(geometry, rpGEOMETRYLOCKVERTICES | rpGEOMETRYLOCKPRELIGHT | rpGEOMETRYLOCKTEXCOORDS);
		
		for ( Int32 i = 0; i < 9; i++ )
		{
			for ( Int32 j = 0; j < 9; j++ )
			{
				(*texCoord).u = Float(i) * 0.125 + TEXTURE_ADDV;
				(*texCoord).v = Float(j) * 0.125 + TEXTURE_ADDU;
				texCoord++;

				RwRGBAAssign(&*prelights, &color);
				prelights++;


				(*vlist).z = ( CWeather::Wind * 0.7f + 0.3f )
							* ( sinf(Float(i + j) * DEG2RAD(45.0f) + fAngle) )
							+ ( CWeather::Wind * 0.2f * sinf(Float(j - i) * PI + (2.0f * fAngle)) );
				vlist++;
			}
		}
		
		RpGeometryUnlock(geometry);
	}
	
	static CBoat *apBoatList[4] = { NULL };
	
	if ( apGeomArray[0] && nGeomUsed < 8
		&& IsSectorAffectedByWake(CVector2D(16.0f + fX, 16.0f + fY), 16.0f, apBoatList) )
	{
		//TODO
		Float v17 = 255.0f - Float(color.blue + color.red + color.green) * 0.33333334f;
		
		if ( v17 > fAdd2 )
			v17 = fAdd2;
		
		Float v44 = fAdd1 - v17;
		
		RpGeometry *wavyGeometry = RpAtomicGetGeometry(ms_pWavyAtomic);	
		RpGeometry *curGeometry = apGeomArray[nGeomUsed++];
		
		RpAtomic *atomic = RpAtomicCreate();
		RpAtomicSetGeometry(atomic, curGeometry, 0);
		
		RwFrame *frame = RwFrameCreate();
		RwMatrixCopy(RwFrameGetMatrix(frame), RwFrameGetMatrix(RpAtomicGetFrame(ms_pWavyAtomic)));
		RpAtomicSetFrame(atomic, frame);
		
		RwTexCoords *curTexCoord = RpGeometryGetVertexTexCoords(curGeometry, rwTEXTURECOORDINATEINDEX0);
		RwTexCoords *wavyTexCoord = RpGeometryGetVertexTexCoords(wavyGeometry, rwTEXTURECOORDINATEINDEX0);
		RwRGBA *curPrelights = RpGeometryGetPreLightColors(curGeometry);
		RwV3d *curVlist = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(curGeometry, 0));
		RwV3d *wavyVlist = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(wavyGeometry, 0));

		RpGeometryLock(curGeometry, rpGEOMETRYLOCKVERTICES | rpGEOMETRYLOCKPRELIGHT | rpGEOMETRYLOCKTEXCOORDS);
		
		for ( Int32 i = 0; i < 9; i++ )
		{
			for ( Int32 j = 0; j < 9; j++ )
			{
				*curTexCoord = *wavyTexCoord;
				curTexCoord++;
				wavyTexCoord++;

				CVector2D v45(Float(i) * 4.0f + fX, Float(j) * 4.0f + fY);

				//TODO
				Float v42 = 0.0f;
		
				for ( Int32 k = 0; k < 4; k++ )
				{
					if ( apBoatList[k] )
						v42 += IsVertexAffectedByWake(CVector(v45.x, v45.y, 0.0f), apBoatList[k]);
				}
				
				if ( v42 > 0.0f )
				{
					RwRGBA wakedColor;
					
					RwRGBAAssign(&wakedColor, &color);

					wakedColor.red   = min(color.red   + Int32(v42 * fRedMult   * v44), 255);
					wakedColor.green = min(color.green + Int32(v42 * fGreenMult * v44), 255);
					wakedColor.blue  = min(color.blue  + Int32(v42 * fBlueMult  * v44), 255);
					
					RwRGBAAssign(&*curPrelights, &wakedColor);

				}
				else
					RwRGBAAssign(&*curPrelights, &color);
				
				curPrelights++;
				
				
				(*curVlist).z = (*wavyVlist).z;

				curVlist++;
				wavyVlist++;
			}
		}
		
		RpGeometryUnlock(curGeometry);
		
		
		RwV3d pos = {0.0f, 0.0f, 0.0f};

		pos.x = fX;
		pos.z = fZ;
		pos.y = fY;

		RwFrameTranslate(RpAtomicGetFrame(atomic), &pos, rwCOMBINEREPLACE);

		RpAtomicRender(atomic);
		
		RpAtomicDestroy(atomic);
		RwFrameDestroy(frame);
	}
	else
	{
		RwV3d pos = { 0.0f, 0.0f, 0.0f };

		pos.x = fX;
		pos.y = fY;
		pos.z = fZ;
	
		RwFrameTranslate(RpAtomicGetFrame(ms_pWavyAtomic), &pos, rwCOMBINEREPLACE);
		
		RpAtomicRender(ms_pWavyAtomic);
	}
}
		
Int16 _RoundValue(Int32 v)
{
	Int16 result = v;
	
	while ( (result & 0x8000) != 0 )
		result += 16;

	while ( result > 16 )
		result -= 16;
	
	return result;
}

inline Bool _IsColideWithBlock(Int32 x, Int32 y, Int32 &block)
{
	block = CWaterLevel::aWaterFineBlockList[x+0][y+0];
	if ( !(block & 0x80) )
		return true;
	
	block = CWaterLevel::aWaterFineBlockList[x+0][y+1];
	if ( !(block & 0x80) )
	{
		block = CWaterLevel::aWaterFineBlockList[x+0][y+2];
		if ( !(block & 0x80) )
			return true;
	}
	
	block = CWaterLevel::aWaterFineBlockList[x+1][y+0];
	if ( !(block & 0x80) )
		return true;
	
	block = CWaterLevel::aWaterFineBlockList[x+1][y+1];
	if ( !(block & 0x80) )
	{
		block = CWaterLevel::aWaterFineBlockList[x+1][y+2];
		if ( !(block & 0x80) )
			return true;
	}
	
	block = CWaterLevel::aWaterFineBlockList[x+2][y+0];
	if ( !(block & 0x80) )
		return true;
	
	block = CWaterLevel::aWaterFineBlockList[x+2][y+1];
	if ( !(block & 0x80) )
	{
		block = CWaterLevel::aWaterFineBlockList[x+2][y+2];
		if ( !(block & 0x80) )
			return true;
	}
	
	return false;
}

void CWaterLevel::RenderBoatWakes(void)
{
}

inline Float _GetWindedWave(Float fX, Float fY)
{
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);	
	Float x = WATER_HUGE_X(fX + 400.0f);
	Float y = WATER_HUGE_Y(fY);
	
	Float fWindFactor (CWeather::WindClipped * 0.4f + 0.2f);
	Float fWave = sin(( (x - floorf(x)) + (y - floorf(y)) ) * _2PI + fAngle);

	return fWindFactor * fWave;
}

void CWaterLevel::RenderWakeSegment(CVector2D &vecA, CVector2D &vecB, CVector2D &vecC, CVector2D &vecD,
		float &fSizeA, float &fSizeB, 
		float &fAlphaA, float &fAlphaB, 
		float &fWakeZ)
{
	/*
	for ( Int32 i = 0; i < 4; i++ )
	{
		if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
			RenderAndEmptyRenderBuffer();
		
		Float fCurStep = (Float)i       / 4;
		Float fNxtStep = (Float)(i + 1) / 4;
		
		Float fLeftCurStep = 1.0f - fCurStep;
		Float fLeftNxtStep = 1.0f - fNxtStep;

		UInt8 AlphaA = (Int32)(fAlphaA * aAlphaFade[i]    );
		UInt8 AlphaB = (Int32)(fAlphaA * aAlphaFade[i + 1]);
		UInt8 AlphaC = (Int32)(fAlphaB * aAlphaFade[i + 1]);
		UInt8 AlphaD = (Int32)(fAlphaB * aAlphaFade[i]    );

		CVector2D PosA = vecB*fCurStep + vecA*fLeftCurStep;
		CVector2D PosB = vecB*fNxtStep + vecA*fLeftNxtStep;
		CVector2D PosC = vecC*fNxtStep + vecD*fLeftNxtStep;
		CVector2D PosD = vecC*fCurStep + vecD*fLeftCurStep;

		Float fUA = (PosA.x / 4) + _TEXTURE_WAKE_ADDU;
		Float fVA = (PosA.y / 4) + _TEXTURE_WAKE_ADDV;

		Float fUB = (PosB.x / 4) + _TEXTURE_WAKE_ADDU;
		Float fVB = (PosB.y / 4) + _TEXTURE_WAKE_ADDV;

		Float fUC = (PosC.x / 4) + _TEXTURE_WAKE_ADDU;
		Float fVC = (PosC.y / 4) + _TEXTURE_WAKE_ADDV;

		Float fUD = (PosD.x / 4) + _TEXTURE_WAKE_ADDU;
		Float fVD = (PosD.y / 4) + _TEXTURE_WAKE_ADDV;
		
#define MIN4(a, b, c, d) (min((a), min((b), min((c), (d)))))
		Float fMinU = floorf(MIN4(fUA, fUB, fUC, fUD));
		Float fMinV = floorf(MIN4(fVA, fVB, fVC, fVD));
#undef MIN4

		Float fZA = _GetWindedWave(PosA.x, PosA.y) + fWakeZ;
		Float fZB = _GetWindedWave(PosB.x, PosB.y) + fWakeZ;
		Float fZC = _GetWindedWave(PosC.x, PosC.y) + fWakeZ;
		Float fZD = _GetWindedWave(PosD.x, PosD.y) + fWakeZ;
	
		Int32 vidx = TempBufferVerticesStored;
	
		RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], PosA.x, PosA.y, fZA);
		RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], fUA - fMinU);
		RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], fVA - fMinV);
		RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 0], 255, 255, 255, AlphaA);

		RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], PosB.x, PosB.y, fZB);
		RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], fUB - fMinU);
		RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], fVB - fMinV);
		RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 1], 255, 255, 255, AlphaB);

		RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], PosC.x, PosC.y, fZC);
		RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], fUC - fMinU);
		RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], fVC - fMinV);
		RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 2], 255, 255, 255, AlphaC);

		RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], PosD.x, PosD.y, fZD);
		RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], fUD - fMinU);
		RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], fVD - fMinV);
		RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 3], 255, 255, 255, AlphaD);
		
		Int32 iidx = TempBufferIndicesStored;
	
		TempBufferRenderIndexList[iidx + 0] = TempBufferVerticesStored + 0;
		TempBufferRenderIndexList[iidx + 1] = TempBufferVerticesStored + 2;
		TempBufferRenderIndexList[iidx + 2] = TempBufferVerticesStored + 1;
		TempBufferRenderIndexList[iidx + 3] = TempBufferVerticesStored + 0;
		TempBufferRenderIndexList[iidx + 4] = TempBufferVerticesStored + 3;
		TempBufferRenderIndexList[iidx + 5] = TempBufferVerticesStored + 2;
		
		TempBufferVerticesStored += 4;
	
		TempBufferIndicesStored += 6;
	}
	*/
}

float CWaterLevel::CalcDistanceToWater(float fX, float fY)
{
	//Float fSectorMaxRenderDist = 250.0f;
	Float fSectorMaxRenderDist = 75.0f;
	
	Int32 nStartX = WATER_TO_SMALL_SECTOR_X(fX - fSectorMaxRenderDist + 400.0f) - 1;
	Int32 nEndX   = WATER_TO_SMALL_SECTOR_X(fX + fSectorMaxRenderDist + 400.0f) + 1;
	Int32 nStartY = WATER_TO_SMALL_SECTOR_Y(fY - fSectorMaxRenderDist         ) - 1;
	Int32 nEndY   = WATER_TO_SMALL_SECTOR_Y(fY + fSectorMaxRenderDist         ) + 1;
	
	nStartX = clamp(nStartX, 0, MAX_SMALL_SECTORS - 1);
	nEndX   = clamp(nEndX,   0, MAX_SMALL_SECTORS - 1);
	nStartY = clamp(nStartY, 0, MAX_SMALL_SECTORS - 1);
	nEndY   = clamp(nEndY,   0, MAX_SMALL_SECTORS - 1);
	
	Float fDistSqr = 1.0e10f;
	
	for ( Int32 x = nStartX; x <= nEndX; x++ )
	{
		for ( Int32 y = nStartY; y <= nEndY; y++ )
		{
			if ( !(aWaterFineBlockList[x][y] & 0x80) )
			{				
				Float fSectorX = WATER_FROM_SMALL_SECTOR_X(x) - 400.0f;
				Float fSectorY = WATER_FROM_SMALL_SECTOR_Y(y);
				
				CVector2D vecDist
				(
					fSectorX + SMALL_SECTOR_SIZE - fX,
					fSectorY + SMALL_SECTOR_SIZE - fY
				);
				
				fDistSqr = min(vecDist.MagnitudeSqr(), fDistSqr);
			}
		}
	}

	return clamp(sqrt(fDistSqr) - 23.0f, 0.0f, fSectorMaxRenderDist);
}

void CWaterLevel::RenderAndEmptyRenderBuffer()
{
	if ( TempBufferVerticesStored )
	{
		LittleTest();

		if ( RwIm3DTransform(TempBufferRenderVertices, TempBufferVerticesStored, NULL, rwIM3D_VERTEXUV) )
		{
			RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, TempBufferRenderIndexList, TempBufferIndicesStored);
			RwIm3DEnd();
		}
	}
	
	TempBufferIndicesStored = 0;
	TempBufferVerticesStored = 0;
}

Bool CWaterLevel::GetGroundLevel(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance)
{
	CColPoint point;
	CEntity *entity;
	
	if ( !CWorld::ProcessVerticalLine(vecPosn + CVector(0.0f, 0.0f, fDistance),
			-fDistance, point, entity, true, false, false, false, true, false, NULL) )
		return false;
		
	*pfOutLevel = point.m_vecPosition.z;
	
	if ( pData != NULL )
	{
		pData->SurfaceType = point.m_nSurfaceTypeB;
		pData->PieceType   = point.m_nPieceTypeB;
	}
	
	return true;
}

Bool CWaterLevel::IsLocationOutOfWorldBounds_WS(CVector const &vecPosn, int nOffset)
{
	Int32 x = Int32((vecPosn.x / 50.0f) + 48.0f);
	Int32 y = Int32((vecPosn.y / 50.0f) + 40.0f);
	
	return x < nOffset || x >= 80 - nOffset || y < nOffset || y >= 80 - nOffset;
}

Bool CWaterLevel::GetGroundLevel_WS(CVector const &vecPosn, float *pfOutLevel, ColData *pData, float fDistance)
{
	if ( IsLocationOutOfWorldBounds_WS(vecPosn, 0) )
		return false;
	else
		return GetGroundLevel(vecPosn, pfOutLevel, pData, fDistance);
}

Bool CWaterLevel::GetWaterDepth(CVector const &vecPosn, float *pfDepth, float *pfLevelNoWaves, float *pfGroundLevel)
{
	Float fLevelNoWaves;
	Float fGroundLevel;
	
	if ( !GetWaterLevelNoWaves(vecPosn.x, vecPosn.y, vecPosn.z, &fLevelNoWaves) )
		return false;
	
	if ( !GetGroundLevel(vecPosn, &fGroundLevel, NULL, 30.0f) )
		fGroundLevel = -100.0;
	
	if ( pfDepth != NULL )
		*pfDepth = fLevelNoWaves - fGroundLevel;

	if ( pfLevelNoWaves != NULL )
		*pfLevelNoWaves = fLevelNoWaves;

	if ( pfGroundLevel != NULL )
		*pfGroundLevel = fGroundLevel;
	
	return true;
}

void CWaterLevel::RenderSeaBirds()
{
	CVector cur_pos = TheCamera.m_sCoords.pos;
	
	if ( !CCullZones::CamNoRain()
		&& !CCullZones::PlayerNoRain()
		&& (CWeather::NewWeatherType == WEATHER_SUNNY || CWeather::NewWeatherType == WEATHER_EXTRA_SUNNY)
		&& CClock::ms_nGameClockHours > 6 && CClock::ms_nGameClockHours < 20 )
	{
		static CVector prev_pos(0.0f, 0.0f, 0.0f);
		static CVector prev_front(0.0f, 0.0f, 0.0f);
		static Int32 timecounter;

		if ( fabs(prev_pos.x - cur_pos.x) + fabs(prev_pos.y - cur_pos.y) + fabs(prev_pos.z - cur_pos.z) > 1.5f )
		{
			prev_pos = cur_pos;
			timecounter = CTimer::m_snTimeInMilliseconds;
		}
		else if ( UInt32(CTimer::m_snTimeInMilliseconds - timecounter) > 5000 )
		{
			static Int32 birdgenTime = 0;
			
			if ( UInt32(CTimer::m_snTimeInMilliseconds - birdgenTime) > 1000 )
			{
				birdgenTime = CTimer::m_snTimeInMilliseconds;
				
				CVector vecPos = cur_pos;
				
				Float fAngle = CGeneral::GetRandomNumberInRange(90.0f, 150.0f);
				
				UInt16 nSinCosIdx = ( CGeneral::GetRandomNumber() % 0xFFFF ) % 1023;
				
				Float fCos = CParticle::m_CosTable[nSinCosIdx];
				Float fSin = CParticle::m_SinTable[nSinCosIdx];

				vecPos.x += (fCos - fSin) * fAngle;
				vecPos.y += (fSin + fCos) * fAngle;
				vecPos.z += CGeneral::GetRandomNumberInRange(10.0f, 30.0f);
				
				CVector vecDir(CGeneral::GetRandomNumberInRange(-1.0f, 1.0f),
								CGeneral::GetRandomNumberInRange(-1.0f, 1.0f),
								0.0f);
				
				CParticle::AddParticle(PARTICLE_BIRD_FRONT, vecPos, vecDir, NULL, 0.0f, 0, 0, 0, 0);
			}
		}
	}
}

void CWaterLevel::RenderShipsOnHorizon()
{
	CVector cur_pos = FindPlayerPed()->m_sCoords.pos;

	static CVector prev_pos(0.0f, 0.0f, 0.0f);
	static CVector prev_front(0.0f, 0.0f, 0.0f);
	static Int32 timecounter;

	if ( fabs(prev_pos.x - cur_pos.x) + fabs(prev_pos.y - cur_pos.y) + fabs(prev_pos.z - cur_pos.z) > 1.5f )
	{
		prev_pos = cur_pos;
		timecounter = CTimer::m_snTimeInMilliseconds;
	}
	else if ( UInt32(CTimer::m_snTimeInMilliseconds - timecounter) > 5000 )
	{
		static Int32 shipgenTime = 0;
		
		if ( UInt32(CTimer::m_snTimeInMilliseconds - shipgenTime) > 4000 )
		{
			shipgenTime = CTimer::m_snTimeInMilliseconds;
			
			CVector vecPos = cur_pos;
			
			Float fAngle = CGeneral::GetRandomNumberInRange(450.0f, 750.0f);
			
			UInt16 nSinCosIdx = ( CGeneral::GetRandomNumber() % 0xFFFF ) % 1023;
			
			Float fCos = CParticle::m_CosTable[nSinCosIdx];
			Float fSin = CParticle::m_SinTable[nSinCosIdx];

			vecPos.x += (fCos - fSin) * fAngle;
			vecPos.y += (fSin + fCos) * fAngle;
			
			Float fLevelNoWaves;
			
			if ( GetWaterLevelNoWaves(vecPos.x, vecPos.y, vecPos.z, &fLevelNoWaves) )
			{
				if ( IsLocationOutOfWorldBounds_WS(vecPos, 1) )
				{
					vecPos.z  = fLevelNoWaves + 9.5f;
					
					CVector vecDir
					(
						CGeneral::GetRandomNumberInRange(-0.1f, 0.1f),
						0.0f,
						0.0f
					);
					
					CParticle::AddParticle(PARTICLE_SHIP_SIDE, vecPos, vecDir,
						NULL, 0.0f, 0, 0, ( CGeneral::GetRandomNumber() % 0xFFFF ) & 7, 0);
				}
			}
		}
	}
}

void CWaterLevel::HandleSeaLifeForms()
{
	if ( CReplay::Mode == 1 )
		return;
	
	CVector cur_pos = FindPlayerPed()->m_sCoords.pos;

	static CVector prev_pos(0.0f, 0.0f, 0.0f);
	static Int32 timecounter;
	
	if ( fabs(prev_pos.x - cur_pos.x) + fabs(prev_pos.y - cur_pos.y) + fabs(prev_pos.z - cur_pos.z) > 1.5f )
	{
		prev_pos = cur_pos;
		timecounter = CTimer::m_snTimeInMilliseconds;
	}
	else if ( UInt32(CTimer::m_snTimeInMilliseconds - timecounter) > 5000 )
	{
		if ( CWaterCreatures::IsSpaceForMoreWaterCreatures() )
		{
			for ( Int32 i = 0; i < 3; i++ )
			{
				CVector vecPos = cur_pos;
				
				Float fAngle = CGeneral::GetRandomNumberInRange(15.0f, 30.0f);
				
				UInt16 nSinCosIdx = ( CGeneral::GetRandomNumber() % 0xFFFF ) % 1023;
				
				Float fCos = CParticle::m_CosTable[nSinCosIdx];
				Float fSin = CParticle::m_SinTable[nSinCosIdx];
				
				vecPos.x += (fCos - fSin) * fAngle;
				vecPos.y += (fSin + fCos) * fAngle;
				
				CWaterCreatures::CreateOne(vecPos, 0xFFFFFFFF);
			}
		}
	}
	
	CWaterCreatures::UpdateAll();
}

void CWaterLevel::HandleBeachToysStuff(void)
{
	;
}



void CWaterLevel::AllocateBoatWakeArray()
{
	RpMorphTarget *morphTarget;
	RwV3d *vlist;
	RpMaterial *triangleMaterial;
	RpTriangle *tlist;
	RpGeometry *baseGeometry;
	RpMorphTarget *baseMorphTarget;


	//CStreaming::MakeSpaceFor(28700); 

	baseGeometry = RpAtomicGetGeometry(ms_pWavyAtomic);
	baseMorphTarget = RpGeometryGetMorphTarget(baseGeometry, 0);
	triangleMaterial = RpGeometryGetMaterial(baseGeometry, 0);


	for ( Int32 i = 0; i < 8; i++ )
	{
		if ( apGeomArray[i] == NULL )
		{
			apGeomArray[i] = RpGeometryCreate(9*9, 128, rpGEOMETRYTRISTRIP | rpGEOMETRYPRELIT | rpGEOMETRYMODULATEMATERIALCOLOR | rpGEOMETRYTEXTURED);

			tlist = RpGeometryGetTriangles(apGeomArray[i]);

			for ( Int32 j = 0; j < 8; j++ )
			{
				for ( Int32 k = 0; k < 8; k++ )
				{
					const RwUInt16 base = (RwUInt16)(j*9+k);

					RpGeometryTriangleSetVertexIndices(apGeomArray[i], tlist, (RwUInt16)base, (RwUInt16)(base+1), (RwUInt16)(base+8+2));
					RpGeometryTriangleSetVertexIndices(apGeomArray[i], (tlist+1), (RwUInt16)base, (RwUInt16)(base+8+2), (RwUInt16)(base+8+1));

					RpGeometryTriangleSetMaterial(apGeomArray[i], tlist, triangleMaterial);
					RpGeometryTriangleSetMaterial(apGeomArray[i], (tlist+1), triangleMaterial);

					tlist+=2;
				}
			}

			morphTarget = RpGeometryGetMorphTarget(apGeomArray[i], 0);
			vlist = RpMorphTargetGetVertices(morphTarget);

			for ( Int32 j = 0; j < 9; j++ )
			{
				for ( Int32 k = 0; k < 9; k++ )
				{
					(*vlist).x = (Float)j * 4.0f;
					(*vlist).y = (Float)k * 4.0f;
					(*vlist).z = 0.0f;
					vlist++;
				}
			}

			RpMorphTargetSetBoundingSphere(morphTarget, RpMorphTargetGetBoundingSphere(baseMorphTarget));			
			RpGeometryUnlock(apGeomArray[i]);
		}
	}
}

void CWaterLevel::FreeBoatWakeArray()
{
	for ( Int32 i = 0; i < 8; i++ )
	{
		if ( apGeomArray[i] )
		{
			RpGeometryDestroy(apGeomArray[i]);
			apGeomArray[i] = NULL;
		}
	}
	
	nGeomUsed = 0;  
}

//CWaterLevel::CreateBeachToy(CVector const&,eBeachToy)	

}