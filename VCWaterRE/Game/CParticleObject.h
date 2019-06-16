#pragma once
#include "types.h"
#include "..\address.h"
#include "CPlaceable.h"
#include "CParticle.h"
#include "CRGBA.h"

#define MAX_PARTICLEOBJECTS 70

enum eParticleObjectType
{	
	POBJECT_PAVEMENT_STEAM							= 0,
	POBJECT_PAVEMENT_STEAM_SLOWMOTION				= 1,
	POBJECT_WALL_STEAM								= 2,
	POBJECT_WALL_STEAM_SLOWMOTION					= 3,
	POBJECT_DARK_SMOKE								= 4,
	POBJECT_FIRE_HYDRANT							= 5,
	POBJECT_CAR_WATER_SPLASH						= 6,
	POBJECT_PED_WATER_SPLASH						= 7,
	POBJECT_SPLASHES_AROUND							= 8,
	POBJECT_SMALL_FIRE								= 9,
	POBJECT_BIG_FIRE								= 10,
	POBJECT_DRY_ICE									= 11,
	POBJECT_DRY_ICE_SLOWMOTION						= 12,
	POBJECT_WATER_FOUNTAIN_VERT						= 13,
	POBJECT_WATER_FOUNTAIN_HORIZ					= 14,
	POBJECT_FIRE_TRAIL								= 15,
	POBJECT_SMOKE_TRAIL								= 16,
	POBJECT_FIREBALL_AND_SMOKE						= 17,
	POBJECT_ROCKET_TRAIL							= 18,
	POBJECT_EXPLOSION_ONCE							= 19,
	POBJECT_CATALINAS_GUNFLASH						= 20,
	POBJECT_CATALINAS_SHOTGUNFLASH					= 21,

};

enum eParticleObjectState
{
	PARTICLEOBJECTSTATE_INITIALISED = 0,
	PARTICLEOBJECTSTATE_UPDATE_CLOSE,
	PARTICLEOBJECTSTATE_UPDATE_FAR,
	PARTICLEOBJECTSTATE_FREE,
};

class CParticleObject : public CPlaceable
{
public:
	CParticleObject *m_pNext;
	CParticleObject *m_pPrev;
	CParticle *m_pParticle;
	Int32 m_nRemoveTimer;
	Int32 m_Type;
	//eParticleObjectType m_Type;
	//tParticleType m_ParticleType;
	Int32 m_ParticleType;
	UInt8 m_nNumEffectCycles;
	UInt8 m_nSkinFrames;
	UInt16 m_nFrameCounter;
	UInt16 m_nState;
	char _pad0[2];
	CVector m_vecTarget;
	Float m_fRandVal;
	Float m_fSize;
	CRGBA m_Color;
	UInt8 m_bRemove;
	UInt8 m_nCreationChance;
	char _pad1[2];
  
	static CParticleObject *&pCloseListHead;
	static CParticleObject *&pFarListHead;
	static CParticleObject *&pUnusedListHead;
	
	static void Initialise();
	static void UpdateAll();
	
	static CParticleObject* AddObject(unsigned short Type, CVector const& vecPos, CVector const& vecTarget, float fSize, unsigned int nLifeTime, RwRGBA const& Color, unsigned char bCanRemove);
};

VALIDATE_SIZE(CParticleObject, 0x84);

//extern CParticleObject (&gPObjectArray)[MAX_PARTICLEOBJECTS];
extern CParticleObject (&gPObjectArray)[MAX_PARTICLEOBJECTS];