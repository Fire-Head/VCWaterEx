#include "waterlevelps2.h"
#include "tweak.h"
#include "utils.h"
#include "Errors.h"
#include "settings.h"

namespace FHPS2
{

#ifdef USE_ORIGINAl_UV
Float &TEXTURE_ADDU = *(Float *)0x77FA6C;
Float &TEXTURE_ADDV = *(Float *)0x77FA70;

Float &_TEXTURE_MASK_ADDU = *(Float *)0x77FA74;
Float &_TEXTURE_MASK_ADDV = *(Float *)0x77FA78;

Float &_TEXTURE_WAKE_ADDU = *(Float *)0x77FA7C;
Float &_TEXTURE_WAKE_ADDV = *(Float *)0x77FA80;

#else

Float TEXTURE_ADDU;
Float TEXTURE_ADDV;

Float _TEXTURE_MASK_ADDU;
Float _TEXTURE_MASK_ADDV;

Float _TEXTURE_WAKE_ADDU;
Float _TEXTURE_WAKE_ADDV;
#endif

#ifndef _USE_ORIGINAL_WATER_VARS
Int32 CWaterLevel::ms_nNoOfWaterLevels;

Float CWaterLevel::ms_aWaterZs[48];
CRect CWaterLevel::ms_aWaterRects[48];

UInt8 CWaterLevel::aWaterBlockList[WATER_BLOCK_SIZE][WATER_BLOCK_SIZE];
UInt8 CWaterLevel::aWaterFineBlockList[WATER_FINEBLOCK_SIZE][WATER_FINEBLOCK_SIZE];

// unused
Bool CWaterLevel::RequireWavySector;
Bool CWaterLevel::MaskCalculatedThisFrame;
CVector CWaterLevel::PreCalculatedMaskPosn;
//

Bool CWaterLevel::WavesCalculatedThisFrame;

Bool CWaterLevel::m_bRenderSeaBed;
Int32 CWaterLevel::m_nRenderWaterLayers;

RpAtomic *CWaterLevel::ms_pWavyAtomic;
RpAtomic *CWaterLevel::ms_pMaskAtomic;

RwTexture *gpWaterTex;
RwTexture *gpWaterEnvTex;
RwTexture *gpWaterWakeTex;

RwRaster *gpWaterRaster;
RwRaster *gpWaterEnvRaster;
RwRaster *gpWaterWakeRaster;

Bool _bSeaLife;
Float _fWaterZOffset = 0.5f;


Int32 TempBufferVerticesStored;
Int32 TempBufferIndicesStored;

#define TEMPBUFFERVERTSIZE 512
#define TEMPBUFFERINDEXSIZE 1024

RwImVertexIndex TempBufferRenderIndexList[TEMPBUFFERINDEXSIZE];
RwIm3DVertex TempBufferRenderVertices[TEMPBUFFERVERTSIZE];

#define RwIm3DVertexSet_RGBA(vert, rgba) RwIm3DVertexSetRGBA(vert, rgba.red, rgba.green, rgba.blue, rgba.alpha) // (RwRGBAAssign(&(_dst)->color, &_src))

#else

Int32 &CWaterLevel::ms_nNoOfWaterLevels = *(Int32*)0x9B6CE8;

Float (&CWaterLevel::ms_aWaterZs)[48] = *(Float (*)[48])*(int *)0x78D658;
CRect (&CWaterLevel::ms_aWaterRects)[48] = *(CRect (*)[48])*(int *)0x860C38;

UInt8 (&CWaterLevel::aWaterBlockList)[WATER_BLOCK_SIZE][WATER_BLOCK_SIZE] = *(UInt8 (*)[WATER_BLOCK_SIZE][WATER_BLOCK_SIZE])*(int *)0x810528;
UInt8 (&CWaterLevel::aWaterFineBlockList)[WATER_FINEBLOCK_SIZE][WATER_FINEBLOCK_SIZE] = *(UInt8 (*)[WATER_FINEBLOCK_SIZE][WATER_FINEBLOCK_SIZE])*(int *)0x864C70;

Bool &CWaterLevel::WavesCalculatedThisFrame = *(Bool*)0xA10B5C;

Bool &CWaterLevel::RequireWavySector = *(Bool*)0xA10B3D;
Bool &CWaterLevel::MaskCalculatedThisFrame = *(Bool*)0xA10B22;
CVector &CWaterLevel::PreCalculatedMaskPosn = *(CVector*)0xA108C0;
Bool &CWaterLevel::m_bRenderSeaBed = *(Bool*)0xA10B86;
Int32 &CWaterLevel::m_nRenderWaterLayers = *(Int32*)0xA10AF2;

RpAtomic *&CWaterLevel::ms_pWavyAtomic = *(RpAtomic **)0xA10860;
RpAtomic *&CWaterLevel::ms_pMaskAtomic = *(RpAtomic **)0xA0D9A8;


RwTexture *&gpWaterTex = *(RwTexture **)0x77FA58;
RwTexture *&gpWaterEnvTex = *(RwTexture **)0x77FA5C;
RwTexture *&gpWaterWakeTex = *(RwTexture **)0x77FA64;

RwRaster *&gpWaterRaster = *(RwRaster **)0x9B6A68;
RwRaster *&gpWaterEnvRaster = *(RwRaster **)0x975214;
RwRaster *&gpWaterWakeRaster = *(RwRaster **)0x983B8C;

Bool &_bSeaLife = *(Bool*)0x77FA84;
Float &_fWaterZOffset = *(Float *)0x69CA90;


Int32 &TempBufferVerticesStored = *(Int32*)0x9B5F74;
Int32 &TempBufferIndicesStored  = *(Int32*)0x97858C;

#define TEMPBUFFERVERTSIZE 512
#define TEMPBUFFERINDEXSIZE 1024

RwImVertexIndex *TempBufferRenderIndexList = (RwImVertexIndex *)0x933BC0;
RwIm3DVertex *TempBufferRenderVertices = (RwIm3DVertex *)0x7E9E08;

#define RwIm3DVertexSet_RGBA(vert, rgba) RwIm3DVertexSetRGBA(vert, rgba.red, rgba.green, rgba.blue, rgba.alpha) // (RwRGBAAssign(&(_dst)->color, &_src))


#endif

SETTWEAKPATH("PS2");
TWEAKFLOAT(fEnvScale,               0.5f, 0.0f,     1.0f,    0.01f);	//	CWaterLevel::CreateWavyAtomic
TWEAKFLOAT(fWave2InvLength,         0.03f, 0.0f,     1.0f,    0.01f);	//	CWaterLevel::PreCalcWavySector
TWEAKFLOAT(fWave2NormScale,         0.5f,  0.0f,     1.0f,    0.01f);	//	CWaterLevel::PreCalcWavySector
TWEAKFLOAT(fWave2Ampl,              0.1f,  0.0f,     1.0f,    0.01f);	//	CWaterLevel::PreCalcWavySector
UInt8 nWaterAlpha             = 192;
UInt8 nWakeAlpha              = 192;
float fUnder1                 = 4.0;
float fUnder2                 = 2.5;
float fUnder3                 = 1.5;
int nMaskAlpha                = 230;
float fAdd1                   = 180.0;
float fAdd2                   = 80.0;
float fRedMult                = 0.6;
float fGreenMult              = 1.0;
float fBlueMult               = 1.4;
float fAlphaMult              = 500.0;
float fAlphaBase              = 30.0;
TWEAKFLOAT(fRandomMoveDiv,          8.0f,   0.0f,     100.0f, 1.0f);	//	CWaterLevel::PreCalcWavySector
TWEAKFLOAT(fRandomDamp,             0.99f,  0.0f,     100.0f, 0.01f);	//	CWaterLevel::PreCalcWavySector
TWEAKFLOAT(fNormMult,               2.0f,   0.0f,     100.0f, 0.01f);	//	CWaterLevel::PreCalcWavySector
TWEAKFLOAT(fNormMultB,              1.0f,   0.0f,     100.0f, 0.01f);	//	CWaterLevel::PreCalcWavySector
float fBumpScale              = 1.5;
float fBumpTexRepeat          = 2.0;
TWEAKFLOAT(fNormalDirectionScalar1, 2.0f,   0.0f,     100.0f, 0.1f);	//	CWaterLevel::PreCalcWavySector
TWEAKFLOAT(fNormalDirectionScalar2, 1.0f,   0.0f,     100.0f, 0.1f);	//	CWaterLevel::PreCalcWavySector
Bool bTestDoNormals           = true;
TWEAKFLOAT(fSeaBedZ,                25.0f,  0.0f,     100.0f, 1.0f);	//	CWaterLevel::RenderOneSlopedUnderWaterPoly
float aAlphaFade[5]               = { 0.4f, 1.0f, 0.2f, 1.0f, 0.4f};	//CWaterLevel::RenderWakeSegment
TWEAKFLOAT(fFlatWaterBlendRange,    0.05f,  0.0f,     1.0f,   0.01f);	//	CWaterLevel::RenderOneFlatSmallWaterPolyBlended
TWEAKFLOAT(fStartBlendDistanceAdd,  64.0f,  0.0f,     100.0f, 1.0f);	//	CWaterLevel::RenderOneFlatSmallWaterPolyBlended
TWEAKFLOAT(fMinWaterAlphaMult,      -30.0f, -100.0f,  100.0f, 1.0f);	//	CWaterLevel::RenderOneFlatSmallWaterPolyBlended



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

inline Float _GetWindedWave(Float fX, Float fY)
{
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);	
	Float x = WATER_HUGE_X(fX + 400.0f);
	Float y = WATER_HUGE_Y(fY);
	
	Float fWindFactor (CWeather::WindClipped * 0.4f + 0.2f);
	Float fWave = sin(( (x - floorf(x)) + (y - floorf(y)) ) * _2PI + fAngle);

	return fWindFactor * fWave;
}

Float SectorRadius(Float fSize)
{
	return sqrtf(powf(fSize, 2) + powf(fSize, 2)) * 1.0040916293f;
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
	Int32 slot = CTxdStore::AddTxdSlot("waterfhps2");
	
	Char Path[MAX_PATH];
	sprintf(Path,"%s\\WaterEx\\PS2\\WATER.TXD", GetAsiPath().c_str());
	
	if ( !file_exists(Path) )
		FATAL("Can't Open: %s", Path);

	CTxdStore::LoadTxd(slot, Path);
	
	CTxdStore::AddRef(slot);
#endif
	
	CTxdStore::SetCurrentTxd(slot);
	
	if ( gpWaterTex == NULL )
		gpWaterTex = RwTextureRead("waterclear256", NULL);
	gpWaterRaster = RwTextureGetRaster(gpWaterTex);
	
	if ( gpWaterEnvTex == NULL )
		gpWaterEnvTex = RwTextureRead("waterreflection2", NULL);
	gpWaterEnvRaster = RwTextureGetRaster(gpWaterEnvTex);
	
	if ( gpWaterWakeTex == NULL )
		gpWaterWakeTex = RwTextureRead("waterwake", NULL);
	gpWaterWakeRaster = RwTextureGetRaster(gpWaterWakeTex);

	CTxdStore::PopCurrentTxd();

	CWaterLevel::CreateWavyAtomic();
	
	printf("Done Initing waterlevels\n");
}

void CWaterLevel::Initialise(char *pWaterCfg)
{
	WaterLevelInitialise(pWaterCfg);
}

void CWaterLevel::Shutdown()
{
	DestroyWavyAtomic();

#define _DELETE_TEXTURE(t) if ( t )	\
		{ \
			RwTextureDestroy(t); \
			t = NULL; \
		}

	_DELETE_TEXTURE(gpWaterTex);
	_DELETE_TEXTURE(gpWaterEnvTex);

#undef _DELETE_TEXTURE

#if 0
	//
#else
	Int32 slot;
	slot = CTxdStore::FindTxdSlot("waterfhps2");
	CTxdStore::RemoveTxdSlot(slot);
#endif
}

void CWaterLevel::CreateWavyAtomic()
{
	RpGeometry *wavyGeometry;
	RpGeometry *maskGeometry;
	RpMaterial *wavyMaterial;
	RpMaterial *maskMaterial;
	
	RpTriangle *wavytlist;
	RpTriangle *masktlist;
	
	RpMorphTarget *wavyMorphTarget;
	RpMorphTarget *maskMorphTarget;
	
	RwSphere boundingSphere;
	
	RwV3d *wavyVert;
	RwV3d *wavyNormal;
	
	RwV3d *maskVert;
	RwV3d *maskNormal;
	
	RwFrame *wavyFrame;
	RwFrame *maskFrame;
	
	{
		wavyGeometry = RpGeometryCreate(17*17, 512, rpGEOMETRYTRISTRIP
													|rpGEOMETRYTEXTURED
													|rpGEOMETRYPRELIT
													|rpGEOMETRYNORMALS
													|rpGEOMETRYMODULATEMATERIALCOLOR);
	}

	{
		maskGeometry = RpGeometryCreate(33*33, 2048, rpGEOMETRYTRISTRIP
													|rpGEOMETRYTEXTURED
													|rpGEOMETRYPRELIT
													|rpGEOMETRYNORMALS
													|rpGEOMETRYMODULATEMATERIALCOLOR);
	}
	
	{
		wavyMaterial = RpMaterialCreate();
		RpMaterialSetTexture(wavyMaterial, gpWaterTex);
		RwRGBA watercolor = { 255, 255, 255, 192 };
		RpMaterialSetColor(wavyMaterial, &watercolor);
	}
	
	{
		maskMaterial = RpMaterialCreate();
		RpMaterialSetTexture(maskMaterial, gpWaterTex);
		RwRGBA watercolor = { 255, 255, 255, 192 };
		RpMaterialSetColor(maskMaterial, &watercolor);
	}
	
	{
		wavytlist = RpGeometryGetTriangles(wavyGeometry);
		
		for ( Int32 i = 0; i < 16; i++ )
		{
			for ( Int32 j = 0; j < 16; j++ )
			{
				const RwUInt16 base = (RwUInt16)((16 + 1)*i+j);
	
				RpGeometryTriangleSetVertexIndices(wavyGeometry, 
						wavytlist,     (RwInt16)base, (RwInt16)(base+1),    (RwInt16)(base+16+2));

				RpGeometryTriangleSetVertexIndices(wavyGeometry, 
						(wavytlist+1), (RwInt16)base, (RwInt16)(base+16+2), (RwInt16)(base+16+1));
	
				RpGeometryTriangleSetMaterial(wavyGeometry, wavytlist,     wavyMaterial);

				RpGeometryTriangleSetMaterial(wavyGeometry, (wavytlist+1), wavyMaterial);
				
				wavytlist+=2;
			}
		}
	}

	{
		masktlist = RpGeometryGetTriangles(maskGeometry);
		
		for ( Int32 i = 0; i < 32; i++ )
		{
			for ( Int32 j = 0; j < 32; j++ )
			{
				const RwUInt16 base = (RwUInt16)((32 + 1)*i+j);
	
				RpGeometryTriangleSetVertexIndices(maskGeometry, 
						masktlist,     (RwInt16)base, (RwInt16)(base+1),    (RwInt16)(base+32+2));

				RpGeometryTriangleSetVertexIndices(maskGeometry,
						(masktlist+1), (RwInt16)base, (RwInt16)(base+32+2), (RwInt16)(base+32+1));
	
				RpGeometryTriangleSetMaterial(maskGeometry, masktlist,     maskMaterial);

				RpGeometryTriangleSetMaterial(maskGeometry, (masktlist+1), maskMaterial);
				
				masktlist+=2;
			}
		}
	}

	{
		wavyMorphTarget = RpGeometryGetMorphTarget(wavyGeometry, 0);
		wavyVert = RpMorphTargetGetVertices(wavyMorphTarget);
		wavyNormal = RpMorphTargetGetVertexNormals(wavyMorphTarget);
		
		for ( Int32 i = 0; i < 17; i++ )
		{
			for ( Int32 j = 0; j < 17; j++ )
			{
				(*wavyVert).x = (Float)i * 2.0f;
				(*wavyVert).y = (Float)j * 2.0f;
				(*wavyVert).z = 0.0f;
				
				(*wavyNormal).x = 0.0f;
				(*wavyNormal).y = 0.0f;
				(*wavyNormal).z = 1.0f;
				
				wavyVert++;
				wavyNormal++;
			}
		}
		
		RpMorphTargetCalcBoundingSphere(wavyMorphTarget, &boundingSphere);
		RpMorphTargetSetBoundingSphere(wavyMorphTarget, &boundingSphere);
		RpGeometryUnlock(wavyGeometry);
	}

	{
		maskMorphTarget = RpGeometryGetMorphTarget(maskGeometry, 0);
		maskVert = RpMorphTargetGetVertices(maskMorphTarget);
		maskNormal = RpMorphTargetGetVertexNormals(maskMorphTarget);
		
		for ( Int32 i = 0; i < 33; i++ )
		{
			for ( Int32 j = 0; j < 33; j++ )
			{
				(*maskVert).x = (Float)i * 2.0f;
				(*maskVert).y = (Float)j * 2.0f;
				(*maskVert).z = 0.0f;
				
				(*maskNormal).x = 0.0f;
				(*maskNormal).y = 0.0f;
				(*maskNormal).z = 1.0f;
				
				maskVert++;
				maskNormal++;
			}
		}
		
		RpMorphTargetCalcBoundingSphere(maskMorphTarget, &boundingSphere);
		RpMorphTargetSetBoundingSphere(maskMorphTarget, &boundingSphere);
		RpGeometryUnlock(maskGeometry);
	}
	
	{
		wavyFrame = RwFrameCreate();
		ms_pWavyAtomic = RpAtomicCreate();
		RpAtomicSetGeometry(ms_pWavyAtomic, wavyGeometry, 0);
		RpAtomicSetFrame(ms_pWavyAtomic, wavyFrame);
		RpMaterialDestroy(wavyMaterial);
		RpGeometryDestroy(wavyGeometry);
	}
	
	{
		maskFrame = RwFrameCreate();
		ms_pMaskAtomic = RpAtomicCreate();
		RpAtomicSetGeometry(ms_pMaskAtomic, maskGeometry, 0);
		RpAtomicSetFrame(ms_pMaskAtomic, maskFrame);
		RpMaterialDestroy(maskMaterial);
		RpGeometryDestroy(maskGeometry);
	}
	
	static RwFrame *wakeEnvFrame;
	
	if ( wakeEnvFrame == NULL )
	{
		wakeEnvFrame = RwFrameCreate();
		RwMatrixSetIdentity(RwFrameGetMatrix(wakeEnvFrame));
		RwFrameUpdateObjects(wakeEnvFrame);
	}
	
	RpMatFXMaterialSetEffects(maskMaterial, rpMATFXEFFECTENVMAP);
	RpMatFXMaterialSetupEnvMap(maskMaterial, gpWaterEnvTex, wakeEnvFrame, TRUE, fEnvScale); // fEnvScale changed
	RpMatFXAtomicEnableEffects(ms_pMaskAtomic);
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
	_DELETE_ATOMIC(ms_pMaskAtomic);
	
#undef _DELETE_ATOMIC
}

Bool CWaterLevel::GetWaterLevel(float fX, float fY, float fZ, float *pfOutLevel, Bool bDontCheckZ)
{
	Int32 x = WATER_HUGE_X(fX + 400.0f);
	Int32 y = WATER_HUGE_Y(fY);
	
#if 1
	if ( x < 0 || x >= 128 )
		return false;
	
	if ( y < 0 || y >= 128 )
		return false;
#endif
	
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
		
	Float fWindFactor = CWeather::WindClipped * 0.4f + 0.2f;
	
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
	
	Float fWindFactor = CWeather::WindClipped * 0.7f + 0.3f;
	
	Float fWave = sinf( Float(Float(4 * y + 4 * x) * (_2PI / MAX_HUGE_SECTORS )) + fAngle );
	
	return fWave * fWindFactor;
}
	   
CVector CWaterLevel::GetWaterNormal(float fX, float fY)
{
	Int32 x = WATER_HUGE_X(fX);
	Int32 y = WATER_HUGE_Y(fY);
	
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	Float fWindFactor = CWeather::WindClipped * 0.4f + 0.2f;
	
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
	
	Float windAddUV = CWeather::WindClipped * 0.0005f + 0.0006f;
	
	Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	
	if ( !CTimer::m_UserPause && !CTimer::m_CodePause )
	{
		TEXTURE_ADDU       += windAddUV;
		TEXTURE_ADDV       += windAddUV;
		
		_TEXTURE_MASK_ADDU 	   += sinf(fAngle) 		 * 0.0005f + 1.1f * windAddUV;
		_TEXTURE_MASK_ADDV 	   -= cosf(fAngle * 1.3f) * 0.0005f + 1.2f * windAddUV;
		
		_TEXTURE_WAKE_ADDU -= sinf(fAngle) 		 * 0.0003f + windAddUV;
		_TEXTURE_WAKE_ADDV += cosf(fAngle * 0.7f) * 0.0003f + windAddUV;
	}
	
	
	if ( _TEXTURE_MASK_ADDU >= 1.0f )
		_TEXTURE_MASK_ADDU = 0.0f;
	if ( _TEXTURE_MASK_ADDV >= 1.0f )
		_TEXTURE_MASK_ADDV = 0.0f;
	
	if ( _TEXTURE_WAKE_ADDU >= 1.0f )
		_TEXTURE_WAKE_ADDU = 0.0f;
	if ( _TEXTURE_WAKE_ADDV >= 1.0f )
		_TEXTURE_WAKE_ADDV = 0.0f;
	
	if ( TEXTURE_ADDU >= 1.0f )
		TEXTURE_ADDU = 0.0f;
	if ( TEXTURE_ADDV >= 1.0f )
		TEXTURE_ADDV = 0.0f;
	

	RwRGBA color = { 0, 0, 0, 255 };
	
	color.red   = (UInt32)CTimeCycle::m_fCurrentWaterRed;
	color.green = (UInt32)CTimeCycle::m_fCurrentWaterGreen;
	color.blue  = (UInt32)CTimeCycle::m_fCurrentWaterBlue;
	
	RwRGBA colorUnderwater = { 0, 0, 0, 255 };
	colorUnderwater.red   = (UInt32)(0.8f * (Float)colorUnderwater.red);
	colorUnderwater.green = (UInt32)(0.8f * (Float)colorUnderwater.green);
	colorUnderwater.blue  = (UInt32)(0.8f * (Float)colorUnderwater.blue);


	TempBufferVerticesStored = 0;
	TempBufferIndicesStored = 0;
	
	WavesCalculatedThisFrame = false;
	
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
				
				if ( fHugeSectorMaxRenderDistSqr > fHugeSectorDistToCamSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecHugeSectorCentre), SectorRadius(HUGE_SECTOR_SIZE), //181.75999f, 181.76
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						
						WavesCalculatedThisFrame = true;
						
						Float fZ;

						if ( !(aWaterBlockList[2*x+0][2*y+0] & 0x80) )
							fZ = ms_aWaterZs[ aWaterBlockList[2*x+0][2*y+0] ];

						if ( !(aWaterBlockList[2*x+1][2*y+0] & 0x80) )
							fZ = ms_aWaterZs[ aWaterBlockList[2*x+1][2*y+0] ];

						if ( !(aWaterBlockList[2*x+0][2*y+1] & 0x80) )
							fZ = ms_aWaterZs[ aWaterBlockList[2*x+0][2*y+1] ];

						if ( !(aWaterBlockList[2*x+1][2*y+1] & 0x80) )
							fZ = ms_aWaterZs[ aWaterBlockList[2*x+1][2*y+1] ];
						
						if ( fHugeSectorDistToCamSqr >= SQR(fHugeSectorNearDist) /*SQR(500.0f)*/ )
						{
							RenderOneFlatHugeWaterPoly(fX, fY, fZ, color);
						}
						else
						{
							if ( m_bRenderSeaBed )
								RenderOneSlopedUnderWaterPoly(fX, fY, fZ, colorUnderwater);
							
							// see RenderTransparentWater()
							;
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
					if ( TheCamera.IsSphereVisible(CVector(vecExtraHugeSectorCentre), SectorRadius(EXTRAHUGE_SECTOR_SIZE), //363.51999f, 363.52
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

	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
	
	if ( WavesCalculatedThisFrame )
	{
		RenderSeaBirds();
		RenderShipsOnHorizon();
		CParticle::HandleShipsAtHorizonStuff();
		HandleBeachToysStuff();
	}
	
	if ( _bSeaLife )
		HandleSeaLifeForms();

	DefinedState();
}



void CWaterLevel::RenderTransparentWater(void)
{
	Bool bUseCamEndX  = false;
	Bool bUseCamStartY = false;
	
	Bool bUseCamStartX = false;
	Bool bUseCamEndY   = false;
	
	_bSeaLife = false;
	
	if ( !CGame::CanSeeWaterFromCurrArea() )
		return;
	
	//TODO:
	/*
	if ( FindPlayerVehicle() && FindPlayerVehicle()->0x2D4 == 1 )
	{
		//f22 = 120.0f;
		
		f22 = 70.0f  + 50.0f;
		f24 = 160.0f + 50.0f;
	}
	else
	{
		f22 = 70.0f;
		f24 = 160.0f;
	}

	*/
	Float fWaterDrawDist      = _GetTransparentWaterDrawDist();
	Float fWaterDrawDistLarge = fWaterDrawDist + 90.0f;
	Float fWavySectorMaxRenderDistSqr   = SQR(fWaterDrawDist);
	
	_GetCamBounds(&bUseCamStartY, &bUseCamEndY, &bUseCamStartX, &bUseCamEndX);

	Float fHugeSectorMaxRenderDist    = _GetWaterDrawDist();
	Float fHugeSectorMaxRenderDistSqr = SQR(fHugeSectorMaxRenderDist);
	
	RenderBoatWakes();
	
	RwRGBA color;
	
	color.red   = (Int32)CTimeCycle::m_fCurrentWaterRed;
	color.green = (Int32)CTimeCycle::m_fCurrentWaterGreen;
	color.blue  = (Int32)CTimeCycle::m_fCurrentWaterBlue;
	color.alpha = 255;
	
	RwRGBA colorTrans;
	
	colorTrans.red   = (Int32)CTimeCycle::m_fCurrentWaterRed;
	colorTrans.green = (Int32)CTimeCycle::m_fCurrentWaterGreen;
	colorTrans.blue  = (Int32)CTimeCycle::m_fCurrentWaterBlue;
	colorTrans.alpha = (Int32)CTimeCycle::m_fCurrentWaterAlpha;
	
	TempBufferVerticesStored = 0;
	TempBufferIndicesStored = 0;
	
	WavesCalculatedThisFrame = false;
	
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)gpWaterRaster);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
	
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
				
				if ( fHugeSectorMaxRenderDistSqr > fHugeSectorDistToCamSqr )
				{
					if ( TheCamera.IsSphereVisible(CVector(vecHugeSectorCentre), SectorRadius(HUGE_SECTOR_SIZE), //181.75999f,
							&CMatrix(TheCamera.GetInverseMatrix())) )
					{
						if ( fHugeSectorDistToCamSqr >= SQR(fHugeSectorNearDist) /*SQR(500.0f)*/ )
						{
							// see RenderWater()
							;
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
													_bSeaLife = true;
													
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
															RenderOneWavySector(fSmallX, fSmallY, fZ, colorTrans);
														else
															RenderOneFlatSmallWaterPolyBlended(fSmallX, fSmallY, fZ, camPos.x, camPos.y, color, colorTrans, fWaterDrawDist);
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
															RenderOneWavySector(fSmallX, fSmallY, fZ, colorTrans);
														else
															RenderOneFlatSmallWaterPolyBlended(fSmallX, fSmallY, fZ, camPos.x, camPos.y, color, colorTrans, fWaterDrawDist);
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
															RenderOneWavySector(fSmallX, fSmallY, fZ, colorTrans);
														else
															RenderOneFlatSmallWaterPolyBlended(fSmallX, fSmallY, fZ, camPos.x, camPos.y, color, colorTrans, fWaterDrawDist);
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
															RenderOneWavySector(fSmallX, fSmallY, fZ, colorTrans);
														else
															RenderOneFlatSmallWaterPolyBlended(fSmallX, fSmallY, fZ, camPos.x, camPos.y, color, colorTrans, fWaterDrawDist);
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
	
	RenderAndEmptyRenderBuffer();


	if ( !CCullZones::WaterFudge() )
	{
		Int32 signX = 0;
		Int32 signY = 0;
		
		Float fCamX = camPos.x - SMALL_SECTOR_SIZE;
		Float fCamY = camPos.y - SMALL_SECTOR_SIZE;
		
		if ( TheCamera.m_sCoords.up.x > 0.3f )
			signX = 1;
		else if ( TheCamera.m_sCoords.up.x < -0.3f )
			signX = -1;
		
		fCamX += 0.3f * (Float)signX * Float(SMALL_SECTOR_SIZE * 2.0f); // 19.2f
		
		if ( TheCamera.m_sCoords.up.y > 0.3f )
			signY = 1;
		else if ( TheCamera.m_sCoords.up.y < -0.3f )
			signY = -1;
			
		fCamY += 0.3f * (Float)signY * Float(SMALL_SECTOR_SIZE * 2.0f); // 19.2f
		
		Int32 nBlock;
		
		Int32 BlockX = WATER_TO_SMALL_SECTOR_X(fCamX + 400.0f) + 1;
		Int32 BlockY = WATER_TO_SMALL_SECTOR_Y(fCamY         ) + 1;
		
		if ( _IsColideWithBlock(BlockX, BlockY, nBlock) )
		{			
			if ( m_nRenderWaterLayers != 1 && m_nRenderWaterLayers != 6 )
			{
				Float fMaskX   = floorf(fCamX / 2.0f) * 2.0f;
				Float fMaskY   = floorf(fCamY / 2.0f) * 2.0f;
				Float fWaterZ  = CWaterLevel::ms_aWaterZs[nBlock];
				Float fSectorX = WATER_FROM_SMALL_SECTOR_X(BlockX) - 400.0f;
				Float fSectorY = WATER_FROM_SMALL_SECTOR_Y(BlockY);
				
				RenderWavyMask(fMaskX, fMaskY, fWaterZ,
						fSectorX, fSectorY,
						signX, signY, colorTrans);
			}
		}
	}
	
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
	
	RwRGBA c;
	
	c.red = color.red;
	c.green = color.green;
	c.blue = color.blue;
	c.alpha = 255;
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 0], c);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + HUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 4.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 1], c);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + HUGE_SECTOR_SIZE, fY + HUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 4.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 4.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 2], c);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + HUGE_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 4.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 3], c);


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
	
	RwRGBA c;
	
	c.red = color.red;
	c.green = color.green;
	c.blue = color.blue;
	c.alpha = 255;
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 0], c);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + EXTRAHUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 8.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 1], c);

	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + EXTRAHUGE_SECTOR_SIZE, fY + EXTRAHUGE_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 8.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 8.0f);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 2], c);
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + EXTRAHUGE_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 8.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSet_RGBA(&TempBufferRenderVertices[vidx + 3], c);


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

void CWaterLevel::RenderOneWavySector(float fX, float fY, float fZ, RwRGBA const &color, Bool bDontRender)
{
	CVector vecSectorPos(fX + (SMALL_SECTOR_SIZE/2), fY + (SMALL_SECTOR_SIZE/2), fZ + 2.0f);

	if ( COcclusion::IsAABoxOccluded(vecSectorPos, SMALL_SECTOR_SIZE, SMALL_SECTOR_SIZE, 4.0f) )
		return;
	
	if ( !WavesCalculatedThisFrame )
	{
		WavesCalculatedThisFrame = true;
		
		Float fAngle = (CTimer::m_snTimeInMilliseconds & 4095) * (_2PI / 4096.0f);
	
		RpGeometry *wavyGeometry   = RpAtomicGetGeometry(ms_pWavyAtomic);
		RwTexCoords *wavyTexCoords = RpGeometryGetVertexTexCoords(wavyGeometry, 1);
		RpMorphTarget *wavyMorph   = RpGeometryGetMorphTarget(wavyGeometry, 0);
		RwRGBA *wavyPreLight       = RpGeometryGetPreLightColors(wavyGeometry);
		RwV3d *wavyMorphVerts      = RpMorphTargetGetVertices(wavyMorph);
		RwV3d *wavyMorphNormals    = RpMorphTargetGetVertexNormals(wavyMorph);

		RpGeometryLock(wavyGeometry, rpGEOMETRYLOCKVERTICES
									|rpGEOMETRYLOCKNORMALS
									|rpGEOMETRYLOCKPRELIGHT
									|rpGEOMETRYLOCKTEXCOORDS);

		RwMatrix *camMat = RwFrameGetLTM(RwCameraGetFrame(RwCameraGetCurrentCamera())); //or curWorld
			
		Float randomDampInv2 = (1.0f - fRandomDamp) * 2.0f;
		
		Float move = 1.0f / 16.0f;
		Float randomMove = 1.0f / (16.0f * fRandomMoveDiv);
		
		Float vertMul = 0.5f;
		
		Float wind     = CWeather::WindClipped * 0.4f + 0.2f;
		Float waveWind = CWeather::WindClipped * fWave2Ampl + 0.05f;
	
		Float waveA = (_2PI / 16.0f)
			* ((fNormalDirectionScalar1 * fabs(camMat->at.x + camMat->at.y) + fNormMult ) * (CWeather::WindClipped * 0.4f + 0.2f));
			
		Float waveB = _2PI / (16.0f * fWave2NormScale)
			* ((fNormalDirectionScalar2 * fabs(camMat->at.y - camMat->at.x) + fNormMultB) * (CWeather::WindClipped * 0.2f + 0.1f));
	
		
		CVector vA
		(
			1.0f,
			0.0f,
			0.0f
		);
		
		CVector vB
		(
			0.0f,
			1.0f,
			0.0f
		);
				
		for ( Int32 i = 0; i < 17; i++ )
		{
			for ( Int32 j = 0; j < 17; j++ )
			{
				wavyTexCoords->u = Float(i) * move + TEXTURE_ADDV;
				wavyTexCoords->v = Float(j) * move + TEXTURE_ADDU;
	
				RwRGBAAssign(wavyPreLight, &color);
				
				if ( i > 0 && i < 16 && j > 0 && j < 16 )
				{
					wavyMorphVerts->x += CGeneral::GetRandomNumberInRange(-1.0f, 1.0f) * randomMove;
					wavyMorphVerts->x *= fRandomDamp;
					wavyMorphVerts->x += Float(i) * randomDampInv2;
					
					wavyMorphVerts->y += CGeneral::GetRandomNumberInRange(-1.0f, 1.0f) * randomMove;
					wavyMorphVerts->y *= fRandomDamp;
					wavyMorphVerts->y += Float(j) * randomDampInv2;
				}
				
				Float morphVertXHalf = ( i == 16 ) ? 0.0f : vertMul * wavyMorphVerts->x;
				Float morphVertYHalf = ( j == 16 ) ? 0.0f : vertMul * wavyMorphVerts->y;
				
				Float waveMulA = (morphVertYHalf + morphVertXHalf) * (_2PI / 16.0f) + fAngle;	  
				Float waveMulB = (morphVertYHalf - morphVertXHalf) * (_2PI / (16.0f * fWave2InvLength)) + fAngle;		
		
				wavyMorphVerts->z = wind * sinf(waveMulA) + waveWind * sin(waveMulB);
				
				vA.z = (waveA * cos(waveMulA)) - (waveB * cos(waveMulB));
				vB.z = (waveA * cos(waveMulA)) + (waveB * cos(waveMulB));
	
				CVector norm = CrossProduct(vA, vB);
				norm.Normalise();
	
				wavyMorphNormals->x = norm.x;
				wavyMorphNormals->y = norm.y;
				wavyMorphNormals->z = norm.z;
	
				++wavyPreLight;
				++wavyTexCoords;
	
				++wavyMorphVerts;
				++wavyMorphNormals;
			}
		}
	
		RpGeometryUnlock(wavyGeometry);
	}
	

	Float fCentreX = fX + (SMALL_SECTOR_SIZE/2);
	Float fCentreY = fY + (SMALL_SECTOR_SIZE/2);
	
	if ( bDontRender == false )
	{
		if ( m_nRenderWaterLayers != 2 && m_nRenderWaterLayers != 4 && m_nRenderWaterLayers != 6 )
		{	
			RwV3d pos = { 0.0f, 0.0f, 0.0f };

			pos.x = fX;
			pos.y = fY;
			pos.z = fZ;
			
			RwFrameTranslate(RpAtomicGetFrame(ms_pWavyAtomic), &pos, rwCOMBINEREPLACE);
			
			RpAtomicRender(ms_pWavyAtomic);
		}
	}
}
		
Int16 _RoundValue(Int32 v)
{
	Int16 result = v;

	while ( result < 0 )
		result += 16;

	while ( result > 16 )
		result -= 16;
	
	return result;
}

void CWaterLevel::RenderWavyMask(float fX, float fY, float fZ,
		float fSectorX, float fSectorY,
		Int32 nCamDirX, Int32 nCamDirY, RwRGBA const&color)
		
{
	Bool bRender = true;
	if ( m_nRenderWaterLayers != 0 && m_nRenderWaterLayers != 2 && m_nRenderWaterLayers != 3 )
		bRender = false;
	

	CVector vecSectorPos(fX + (LARGE_SECTOR_SIZE/2), fY + (LARGE_SECTOR_SIZE/2), fZ + 2.0f);

	
	if ( COcclusion::IsAABoxOccluded(vecSectorPos, LARGE_SECTOR_SIZE, LARGE_SECTOR_SIZE, 4.0f) )
		return;
	
	Float fUOffset = fX - (MAX_LARGE_SECTORS * (Int32)floorf(fX / MAX_LARGE_SECTORS));
	Float fVOffset = fY - (MAX_LARGE_SECTORS * (Int32)floorf(fY / MAX_LARGE_SECTORS));
	
	Int32 nSecsX = (Int32)((fX - fSectorX) / 2.0f);
	Int32 nSecsY = (Int32)((fY - fSectorY) / 2.0f);
	
	
	
	RpGeometry    *wavyGeometry  = RpAtomicGetGeometry(ms_pWavyAtomic);
	RpMorphTarget *wavyMorph     = RpGeometryGetMorphTarget(wavyGeometry, 0);
	RwV3d *wavyMorphVerts        = RpMorphTargetGetVertices(wavyMorph);
	RwV3d *wavyMorphNormals      = RpMorphTargetGetVertexNormals(wavyMorph);

	RpGeometry    *maskGeometry  = RpAtomicGetGeometry(ms_pMaskAtomic);
	RwTexCoords   *maskTexCoords = RpGeometryGetVertexTexCoords(maskGeometry, 1);
	RwRGBA        *maskPreLight  = RpGeometryGetPreLightColors(maskGeometry);
	RpMorphTarget *maskMorph     = RpGeometryGetMorphTarget(maskGeometry, 0);
	RwV3d *maskMorphVerts        = RpMorphTargetGetVertices(maskMorph);
	RwV3d *maskMorphNormals      = RpMorphTargetGetVertexNormals(maskMorph);

	RpGeometryLock(maskGeometry, rpGEOMETRYLOCKVERTICES
								|rpGEOMETRYLOCKNORMALS
								|rpGEOMETRYLOCKPRELIGHT
								|rpGEOMETRYLOCKTEXCOORDS);
	
	RpMaterial *maskMat = RpGeometryGetMaterial(maskGeometry, 0);
	RpMatFXMaterialSetEnvMapFrame(maskMat, RwCameraGetFrame(RwCameraGetCurrentCamera()));
	RpMatFXMaterialSetEnvMapCoefficient(maskMat, fEnvScale);
	RpMatFXMaterialSetEnvMapFrameBufferAlpha(maskMat, TRUE);

	
	Float fMinSparkZ = (CWeather::WindClipped * fWave2Ampl + 0.05f + 
			CWeather::WindClipped * 0.4f + 0.2) * (1.0f - 0.04f * CWeather::SunGlare);

	Int32 randval = CGeneral::GetRandomNumber();

	Float fUVStep = 0.125f;
	Float f27 = 2.0f;
	
	Float fMinU = (fUOffset / 16.0f) + _TEXTURE_MASK_ADDU;
	Float fMinV = (fVOffset / 16.0f) + _TEXTURE_MASK_ADDV;
	
		
	Float fAlphaMul = ((Float)color.alpha * 0.4f) / 16.0f;
	
	
	Float fXOffset = 16.0f;
	if ( nCamDirX > 0 )
		fXOffset = 6.4f;
	else if ( nCamDirX < 0 )
		fXOffset = 25.6f;
	
	Float fYOffset = 16.0f;
	if ( nCamDirY > 0 )
		fYOffset = 6.4f;
	else if ( nCamDirY < 0 )
		fYOffset = 25.6f;


	Int16 nX = _RoundValue(nSecsX - 1);
	Int16 nY = _RoundValue(nSecsY - 1);
	//
	Int16 idxX = nX;
	
	for ( Int32 i = 0; i < 17; i++ )
	{
		Int16 idxY = nY;

		if ( ++idxX > 16 )
				idxX -= 16;
			
		for ( Int32 j = 0; j < 17; j++ )
		{
			if ( ++idxY > 16 )
				idxY -= 16;
			
			const Int32 a = (0*16);
			const Int32 b = (1*16);
			const Int32 c = (33*16);
			const Int32 d = (34*16);
			
			Int32 base = (i*33+j);
			
			maskTexCoords[base+a].u = fMinU + ((Float)i * fUVStep);
			maskTexCoords[base+a].v = fMinV + ((Float)j * fUVStep);
			
			maskTexCoords[base+b].u = maskTexCoords[base+a].u;
			maskTexCoords[base+b].v = maskTexCoords[base+a].v + (16.0f * fUVStep);
			
			maskTexCoords[base+c].u = maskTexCoords[base+a].u + (16.0f * fUVStep);
			maskTexCoords[base+c].v = maskTexCoords[base+a].v;
			
			maskTexCoords[base+d].u = maskTexCoords[base+a].u + (16.0f * fUVStep);
			maskTexCoords[base+d].v = maskTexCoords[base+a].v + (16.0f * fUVStep);
			
			
			maskMorphVerts[base+a].x = (wavyMorphVerts[idxY + (17 * idxX)].x - (Float)idxX * 2.0f) + (Float(i) * 2.0f);
			maskMorphVerts[base+b].x = maskMorphVerts[base+a].x;
			maskMorphVerts[base+c].x = maskMorphVerts[base+a].x + 32.0f;
			maskMorphVerts[base+d].x = maskMorphVerts[base+c].x;
			
			maskMorphVerts[base+a].y = (wavyMorphVerts[idxY + (17 * idxX)].y - (Float)idxY * 2.0f) + (Float(j) * 2.0f);
			maskMorphVerts[base+c].y = maskMorphVerts[base+a].y;
			maskMorphVerts[base+b].y = maskMorphVerts[base+a].y + 32.0f;
			maskMorphVerts[base+d].y = maskMorphVerts[base+b].y;
			
			maskMorphVerts[base+a].z = wavyMorphVerts[idxY + (17 * idxX)].z;
			maskMorphVerts[base+d].z = maskMorphVerts[base+a].z;
			maskMorphVerts[base+c].z = maskMorphVerts[base+d].z;
			maskMorphVerts[base+b].z = maskMorphVerts[base+c].z;
			
			
			if ( maskMorphVerts[base].z >= fMinSparkZ )
			{
				switch ( (i + j + randval) & 3 )
				{
					case 0:
						{
							CVector vecPos
							(
								fX + maskMorphVerts[base+a].x,
								fY + maskMorphVerts[base+a].y,
								fZ + maskMorphVerts[base+a].z + 0.12f
							);

							vecPos -= 0.05f * TheCamera.m_sCoords.up;
							
							CParticle::AddParticle(PARTICLE_WATER_SPARK,
								vecPos,
								CVector(0.0f, 0.0f, 0.0f),
								NULL,
								0.0f,
								15,
								CGeneral::GetRandomNumberInRange(-90, 90),
								0,
								0);
						}
						break;

					case 1:
						{
							CVector vecPos
							(
								fX + maskMorphVerts[base+c].x,
								fY + maskMorphVerts[base+c].y,
								fZ + maskMorphVerts[base+c].z + 0.12f
							);

							vecPos -= 0.05f * TheCamera.m_sCoords.up;
							
							CParticle::AddParticle(PARTICLE_WATER_SPARK,
								vecPos,
								CVector(0.0f, 0.0f, 0.0f),
								NULL,
								0.0f,
								15,
								CGeneral::GetRandomNumberInRange(-90, 90),
								0,
								0);
						}
						break;

					case 2:
						{
							CVector vecPos
							(
								fX + maskMorphVerts[base+b].x,
								fY + maskMorphVerts[base+b].y,
								fZ + maskMorphVerts[base+b].z + 0.12f
							);

							vecPos -= 0.05f * TheCamera.m_sCoords.up;
							
							CParticle::AddParticle(PARTICLE_WATER_SPARK,
								vecPos,
								CVector(0.0f, 0.0f, 0.0f),
								NULL,
								0.0f,
								15,
								CGeneral::GetRandomNumberInRange(-90, 90),
								0,
								0);
						}
						break;

					case 3:
						{
							CVector vecPos
							(
								fX + maskMorphVerts[base+d].x,
								fY + maskMorphVerts[base+d].y,
								fZ + maskMorphVerts[base+d].z + 0.12f
							);

							vecPos -= 0.05f * TheCamera.m_sCoords.up;
							
							CParticle::AddParticle(PARTICLE_WATER_SPARK,
								vecPos,
								CVector(0.0f, 0.0f, 0.0f),
								NULL,
								0.0f,
								15,
								CGeneral::GetRandomNumberInRange(-90, 90),
								0,
								0);
						}
						break;
				}
			}

			maskMorphNormals[base+a].x = wavyMorphNormals[idxY + (17 * idxX)].x;
			maskMorphNormals[base+a].y = wavyMorphNormals[idxY + (17 * idxX)].y;
			maskMorphNormals[base+a].z = wavyMorphNormals[idxY + (17 * idxX)].z;
			
			maskMorphNormals[base+d].x = maskMorphNormals[base+a].x;
			maskMorphNormals[base+d].y = maskMorphNormals[base+a].y;
			maskMorphNormals[base+d].z = maskMorphNormals[base+a].z;
			
			maskMorphNormals[base+c].x = maskMorphNormals[base+d].x;
			maskMorphNormals[base+c].y = maskMorphNormals[base+d].y;
			maskMorphNormals[base+c].z = maskMorphNormals[base+d].z;
			
			maskMorphNormals[base+b].x = maskMorphNormals[base+c].x;
			maskMorphNormals[base+b].y = maskMorphNormals[base+c].y;
			maskMorphNormals[base+b].z = maskMorphNormals[base+c].z;
	  
			maskPreLight[base+a].red   = color.red;
			maskPreLight[base+a].green = color.green;
			maskPreLight[base+a].blue  = color.blue;
			maskPreLight[base+a].alpha = color.alpha;
			
			maskPreLight[base+d].red   = maskPreLight[base+a].red;
			maskPreLight[base+d].green = maskPreLight[base+a].green;
			maskPreLight[base+d].blue  = maskPreLight[base+a].blue;
			maskPreLight[base+d].alpha = maskPreLight[base+a].alpha;
			
			maskPreLight[base+c].red   = maskPreLight[base+d].red;
			maskPreLight[base+c].green = maskPreLight[base+d].green;
			maskPreLight[base+c].blue  = maskPreLight[base+d].blue;
			maskPreLight[base+c].alpha = maskPreLight[base+d].alpha;
			
			maskPreLight[base+b].red   = maskPreLight[base+c].red;
			maskPreLight[base+b].green = maskPreLight[base+c].green;
			maskPreLight[base+b].blue  = maskPreLight[base+c].blue;
			maskPreLight[base+b].alpha = maskPreLight[base+c].alpha;
			
			
			maskPreLight[base+a].alpha = max( 0, (Int32)((Float)color.alpha - (fAlphaMul * ( fabs(        (Float)i - fXOffset) + fabs(        (Float)j - fYOffset  ) ) )) );
			maskPreLight[base+b].alpha = max( 0, (Int32)((Float)color.alpha - (fAlphaMul * ( fabs(        (Float)i - fXOffset) + fabs(16.0f + (Float)j - fYOffset  ) ) )) );
			maskPreLight[base+c].alpha = max( 0, (Int32)((Float)color.alpha - (fAlphaMul * ( fabs(16.0f + (Float)i - fXOffset) + fabs(        (Float)j - fYOffset  ) ) )) );
			maskPreLight[base+d].alpha = max( 0, (Int32)((Float)color.alpha - (fAlphaMul * ( fabs(16.0f + (Float)i - fXOffset) + fabs(16.0f + (Float)j - fYOffset  ) ) )) );
	
		}
	}
	//
	
	RpGeometryUnlock(maskGeometry);
	
	{
		RwV3d pos = { 0.0f, 0.0f, 0.0f };
		
		pos.x = fX;
		pos.y = fY;
		pos.z = fZ + 0.05f;
		
		RwFrameTranslate(RpAtomicGetFrame(ms_pMaskAtomic), &pos, rwCOMBINEREPLACE);
		
		if ( bRender )
		{
			//RpSkyTexCacheFlush();
			RpAtomicRender(ms_pMaskAtomic);
		}
	}
}

void CWaterLevel::RenderBoatWakes(void)
{
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)gpWaterWakeRaster);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	CBoat::FillBoatList();
	
	Float fWakeZ = 5.97f;
	Float fWakeLifeTimeMult = 0.01f / CBoat::WAKE_LIFETIME;
	
	for ( Int32 idx = 0; idx < 4; idx++ )
	{
		CBoat *pBoat = CBoat::apFrameWakeGeneratingBoats[idx];

		if ( pBoat == NULL )
			break;

		CVector2D vecDistA
		(
			pBoat->m_sCoords.up.x,
			pBoat->m_sCoords.up.y
		);


		Float fSize = CModelInfo::ms_modelInfoPtrs[pBoat->m_nModelIndex]->m_pColModel->m_boundBox.m_vecMax.x
				* 0.65f;
		
		if ( pBoat->m_nModelIndex == 190 )
			fSize *= 0.4f;
		
		Float fAplhaA = 255.0f;
		Float fSizeA = fSize;
		Float fAplhaB;
		Float fSizeB;
		
		for ( Int32 wake = 1; wake < pBoat->m_nNumWaterTrailPoints; wake++ )
		{
			Bool bRender = true;
			
			Float fTimeleft = CBoat::WAKE_LIFETIME - pBoat->m_afWakePointLifeTime[wake];

			Float fWakeSizeB = ((Float)wake * 0.19f) + fSize - fWakeLifeTimeMult * max(fTimeleft, 0.0f);
			
			fSizeB = fWakeSizeB / CBoat::MIN_WAKE_INTERVAL;
			if ( fSizeB < 0.0f )
				fSizeB = 1.0f;
			
			if ( wake == pBoat->m_nNumWaterTrailPoints - 1 )
			{
				// set alpha to 0 if it's last point
				fAplhaB = 0.0f;
			}
			else
			{
				// clip (-100, 500), less lifetime - less val
				Float val = 500.0f - (CBoat::WAKE_LIFETIME - pBoat->m_afWakePointLifeTime[wake])
						* 600.0f / CBoat::WAKE_LIFETIME;
						
				fAplhaB = clamp(val, 0.0f, 255.0f);
			}

			CVector2D vecDistB = pBoat->m_avecWakePoints[wake - 1] - pBoat->m_avecWakePoints[wake];			

			Float fScal = vecDistB.MagnitudeSqr();
			
			// normalize if distance between points is greater than 3

			if ( fScal > SQR(3.0f) )
			{				
				Float fNorm = 1.0f / sqrt(fScal);
				
				vecDistB.x *= fNorm;
				vecDistB.y *= fNorm;
				
				// disable render if distance between points too big
				
				if ( sqrt(fScal) > 13.0f )
					bRender = false;
			}
			
			CVector2D vecAA
			(
				pBoat->m_avecWakePoints[wake - 1].x - (fSizeA * vecDistA.y),
				pBoat->m_avecWakePoints[wake - 1].y + (fSizeA * vecDistA.x)
			);
			CVector2D vecAB
			(
				pBoat->m_avecWakePoints[wake - 1].x + (fSizeA * vecDistA.y),
				pBoat->m_avecWakePoints[wake - 1].y - (fSizeA * vecDistA.x)
			);
			CVector2D vecBA
			(
				pBoat->m_avecWakePoints[wake].x     + (fSizeB * vecDistB.y),
				pBoat->m_avecWakePoints[wake].y     - (fSizeB * vecDistB.x)
			);                                                                            
			CVector2D vecBB                         
			(                                       
				pBoat->m_avecWakePoints[wake].x     - (fSizeB * vecDistB.y),
				pBoat->m_avecWakePoints[wake].y     + (fSizeB * vecDistB.x)
			);
			
			if ( bRender )
				RenderWakeSegment(vecAA, vecAB, vecBA, vecBB, fSizeA, fSizeB, fAplhaA, fAplhaB, fWakeZ);
			
			vecDistA = vecDistB;
			fSizeA = fSizeB;
			
			fAplhaB = fAplhaA;
		}
	}
	
	RenderAndEmptyRenderBuffer();
}

void CWaterLevel::RenderWakeSegment(CVector2D &vecA, CVector2D &vecB, CVector2D &vecC, CVector2D &vecD,
		float &fSizeA, float &fSizeB, 
		float &fAlphaA, float &fAlphaB, 
		float &fWakeZ)
{
	for ( Int32 i = 0; i < 4; i++ )
	{
		if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
			RenderAndEmptyRenderBuffer();
		
		Float fCurStep = (Float)i       / 4;
		Float fNxtStep = (Float)(i + 1) / 4;
		
		Float fLeftCurStep = 1.0f - fCurStep;
		Float fLeftNxtStep = 1.0f - fNxtStep;

		UInt8 AlphaA = (UInt32)(fAlphaA * aAlphaFade[i]    );
		UInt8 AlphaB = (UInt32)(fAlphaA * aAlphaFade[i + 1]);
		UInt8 AlphaC = (UInt32)(fAlphaB * aAlphaFade[i + 1]);
		UInt8 AlphaD = (UInt32)(fAlphaB * aAlphaFade[i]    );

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
}

void CWaterLevel::RenderOneSlopedUnderWaterPoly(float fX, float fY, float fZ, RwRGBA const&color)
{
	CVector2D camPos
	(
		TheCamera.m_sCoords.pos.x,
		TheCamera.m_sCoords.pos.y
	);
	
	Float fDistA = (CVector2D(fX, fY) - camPos).Magnitude()                                       + -140.0f;
	Float fDistB = (CVector2D(fX, fY + HUGE_SECTOR_SIZE) - camPos).Magnitude()                    + -140.0f;
	Float fDistC = (CVector2D(fX + HUGE_SECTOR_SIZE, fY + HUGE_SECTOR_SIZE) - camPos).Magnitude() + -140.0f;
	Float fDistD = (CVector2D(fX + HUGE_SECTOR_SIZE, fY) - camPos).Magnitude()                    + -140.0f;
	/*
	#define CALCSEABED(v, d)	\
		{	\
		v = 0.1f;	\
		if ( d < 0.0f )	\
			v += fSeaBedZ;	\
		else if ( d <= 240.0f )	\
			v += (fSeaBedZ / 240.0f) * (240.0f - d);	\
		}
	*/
	#define CALCSEABED(v, d) \
		{ \
			if ( d < 0.0f ) \
				v = 0.1f + fSeaBedZ; \
			else if ( d > 240.0f ) \
				v = 0.1f; \
			else \
				v = 0.1f + ((fSeaBedZ * (240.0f - d)) / 240.0f); \
		}
	
	Float fSeaBedA, fSeaBedB, fSeaBedC, fSeaBedD;
	
	CALCSEABED(fSeaBedA, fDistA);
	CALCSEABED(fSeaBedB, fDistB);
	CALCSEABED(fSeaBedC, fDistC);
	CALCSEABED(fSeaBedD, fDistD);
	
	#undef CALCSEABED
	
	if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
		RenderAndEmptyRenderBuffer();
	
	Int32 vidx = TempBufferVerticesStored;
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset - fSeaBedA);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], 0.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], 0.0f);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 0], color.red, color.green, color.blue, 255);
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + HUGE_SECTOR_SIZE, fZ - _fWaterZOffset - fSeaBedB);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], 0.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], 4.0f);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 1], color.red, color.green, color.blue, 255);
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + HUGE_SECTOR_SIZE, fY + HUGE_SECTOR_SIZE, fZ - _fWaterZOffset - fSeaBedC);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], 4.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], 4.0f);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 2], color.red, color.green, color.blue, 255);
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + HUGE_SECTOR_SIZE, fY, fZ - _fWaterZOffset - fSeaBedD);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], 4.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], 0.0f);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 3], color.red, color.green, color.blue, 255);
	
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

void CWaterLevel::RenderOneFlatSmallWaterPolyBlended(float fX, float fY, float fZ, float fCamX, float fCamY,
		RwRGBA const &color, RwRGBA const &colorTrans,
		float fDrawDist)
{
	if ( TempBufferIndicesStored >= TEMPBUFFERINDEXSIZE-6 || TempBufferVerticesStored >= TEMPBUFFERVERTSIZE-4 )
		RenderAndEmptyRenderBuffer();
	
	Int32 vidx = TempBufferVerticesStored;
	
	Float fBlendDrawDist = fDrawDist + fStartBlendDistanceAdd;
	
	Float fDistStartX = SQR(fX                       - fCamX);
	Float fDistStartY = SQR(fY                       - fCamY);
	Float fDistEndX   = SQR((fX + SMALL_SECTOR_SIZE) - fCamX);
	Float fDistEndY   = SQR((fY + SMALL_SECTOR_SIZE) - fCamY);
	
	Float fAlphaBlendMulA 
			= min(fFlatWaterBlendRange * max(sqrt(fDistStartX + fDistStartY) - fBlendDrawDist, fMinWaterAlphaMult), 1.0f);
	Float fAlphaBlendMulB
			= min(fFlatWaterBlendRange * max(sqrt(fDistStartX + fDistEndY  ) - fBlendDrawDist, fMinWaterAlphaMult), 1.0f);
	Float fAlphaBlendMulC
			= min(fFlatWaterBlendRange * max(sqrt(fDistEndX   + fDistEndY  ) - fBlendDrawDist, fMinWaterAlphaMult), 1.0f);
	Float fAlphaBlendMulD
			= min(fFlatWaterBlendRange * max(sqrt(fDistEndX   + fDistStartY) - fBlendDrawDist, fMinWaterAlphaMult), 1.0f);


	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 0], fX, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 0], TEXTURE_ADDV);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 0], color.red, color.green, color.blue,
		( colorTrans.alpha + ((color.alpha - colorTrans.alpha) * (UInt8)(Int32)fAlphaBlendMulA) )  );
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 1], fX, fY + SMALL_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDU);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 1], TEXTURE_ADDV + 1.0f);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 1], color.red, color.green, color.blue,
		( colorTrans.alpha + ((color.alpha - colorTrans.alpha) * (UInt8)(Int32)fAlphaBlendMulB) ) );
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 2], fX + SMALL_SECTOR_SIZE, fY + SMALL_SECTOR_SIZE, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDU + 1.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 2], TEXTURE_ADDV + 1.0f);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 2], color.red, color.green, color.blue,
		( colorTrans.alpha + ((color.alpha - colorTrans.alpha) * (UInt8)(Int32)fAlphaBlendMulC) ) );
	
	RwIm3DVertexSetPos  (&TempBufferRenderVertices[vidx + 3], fX + SMALL_SECTOR_SIZE, fY, fZ - _fWaterZOffset);
	RwIm3DVertexSetU    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDU + 1.0f);
	RwIm3DVertexSetV    (&TempBufferRenderVertices[vidx + 3], TEXTURE_ADDV);
	RwIm3DVertexSetRGBA (&TempBufferRenderVertices[vidx + 3], color.red, color.green, color.blue,
		( colorTrans.alpha + ((color.alpha - colorTrans.alpha) * (UInt8)(Int32)fAlphaBlendMulD) ) );


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

float CWaterLevel::CalcDistanceToWater(float fX, float fY)
{
	Float fSectorMaxRenderDist = 250.0f;
	
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
	CVector cur_pos = FindPlayerPed()->m_sCoords.pos;
	
	static Bool bBeachBallInit = true;
	static CVector FirstBeachBallPos = cur_pos;
	static Bool bLoungeInit = true;
	static CVector FirstLoungePos = cur_pos;
	static CVector prev_pos(0.0f, 0.0f, 0.0f);
	static Int32 timecounter;

	if ( fabs(prev_pos.x - cur_pos.x) + fabs(prev_pos.y - cur_pos.y) + fabs(prev_pos.z - cur_pos.z) > 1.5f )
	{
		prev_pos = cur_pos;
		timecounter = CTimer::m_snTimeInMilliseconds;
	}
	else if ( UInt32(CTimer::m_snTimeInMilliseconds - timecounter) > 5000 )
	{
		static Int32 toygenTime = CTimer::m_snTimeInMilliseconds;
		
		if ( UInt32(CTimer::m_snTimeInMilliseconds - toygenTime) > 20000 )
		{
			toygenTime = CTimer::m_snTimeInMilliseconds;
			
			if ( bBeachBallInit ||  (cur_pos - FirstBeachBallPos).MagnitudeSqr() > 6400.0f )
			{
				for ( Int32 i = 0; i < 3; i++ )
				{
					CVector vecPos = cur_pos;
					
					Float fAngle = CGeneral::GetRandomNumberInRange(20.0f, 35.0f);
					
					UInt16 nSinCosIdx = CGeneral::GetRandomNumber() % 1023;
					
					Float fCos = CParticle::m_CosTable[nSinCosIdx];
					Float fSin = CParticle::m_SinTable[nSinCosIdx];
					
					vecPos.x += (fCos - fSin) * fAngle;
					vecPos.y += (fSin + fCos) * fAngle;
					
					if ( TheCamera.IsSphereVisible(vecPos, 1.0f, &CMatrix(TheCamera.GetInverseMatrix())) )
					{
						Float fWaterLevel;
						
						if ( !GetWaterLevel(vecPos.x, vecPos.y, vecPos.z, &fWaterLevel, false) )
						{
							Float fGroundLevel;
							ColData coldata;
							
							if ( GetGroundLevel(vecPos, &fGroundLevel, &coldata, 30.0f) )
							{
								if ( coldata.SurfaceType == 18 )
								{
									CEntity *toy = CreateBeachToy(vecPos, BEACHTOY_BALL);
									
									if ( toy )
									{
										FirstBeachBallPos = cur_pos;
										bBeachBallInit = false;
										i = 10;
									}
								}
							}
						}
					}
				}
			}
			
			if ( bLoungeInit ||  (cur_pos - FirstLoungePos).MagnitudeSqr() > 6400.0f )
			{
				for ( Int32 i = 0; i < 5; i++ )
				{
					CVector vecPos = cur_pos;
					
					Float fAngle = CGeneral::GetRandomNumberInRange(20.0f, 35.0f);
					
					UInt16 nSinCosIdx = CGeneral::GetRandomNumber() % 1023;
					
					Float fCos = CParticle::m_CosTable[nSinCosIdx];
					Float fSin = CParticle::m_SinTable[nSinCosIdx];
					
					vecPos.x += (fCos - fSin) * fAngle;
					vecPos.y += (fSin + fCos) * fAngle;
					
					if ( TheCamera.IsSphereVisible(vecPos, 2.0f, &CMatrix(TheCamera.GetInverseMatrix())) )
					{
						Float fWaterLevel;
						
						if ( !GetWaterLevel(vecPos.x, vecPos.y, vecPos.z, &fWaterLevel, false) )
						{
							Float fGroundLevel;
							ColData coldata;
							
							if ( GetGroundLevel(vecPos, &fGroundLevel, &coldata, 30.0f) )
							{
								if ( coldata.SurfaceType == 18 )
								{
									CEntity *toy = CreateBeachToy(vecPos, BEACHTOY_LOUNGE);
									if ( toy )
									{
										toy->SetHeading(DEG2RAD(CGeneral::GetRandomNumberInRange(0.0f, 359.0f)));
										FirstLoungePos = cur_pos;
										bLoungeInit = false;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

CEntity *CWaterLevel::CreateBeachToy(CVector const &vec, eBeachToy beachtoy)
{
	// fuck that, i'm tired
	return ((CEntity * (__cdecl *)(CVector const&, eBeachToy))0x5BB570)(vec, beachtoy);
}

}