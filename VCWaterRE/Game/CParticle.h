#pragma once
#include <windows.h>
#include "..\address.h"
#include "CVector.h"
#include "RenderWare.h"
#include "cParticleSystemMgr.h"

enum tParticleType
{
  PARTICLE_SPARK = 0x0,
  PARTICLE_SPARK_SMALL = 0x1,
  PARTICLE_WATER_SPARK = 0x2,
  PARTICLE_WHEEL_DIRT = 0x3,
  PARTICLE_SAND = 0x4,
  PARTICLE_WHEEL_WATER = 0x5,
  PARTICLE_BLOOD = 0x6,
  PARTICLE_BLOOD_SMALL = 0x7,
  PARTICLE_BLOOD_SPURT = 0x8,
  PARTICLE_DEBRIS = 0x9,
  PARTICLE_DEBRIS2 = 0xA,
  PARTICLE_FLYERS = 0xB,
  PARTICLE_WATER = 0xC,
  PARTICLE_FLAME = 0xD,
  PARTICLE_FIREBALL = 0xE,
  PARTICLE_GUNFLASH = 0xF,
  PARTICLE_GUNFLASH_NOANIM = 0x10,
  PARTICLE_GUNSMOKE = 0x11,
  PARTICLE_GUNSMOKE2 = 0x12,
  PARTICLE_CIGARETTE_SMOKE = 0x13,
  PARTICLE_SMOKE = 0x14,
  PARTICLE_SMOKE_SLOWMOTION = 0x15,
  PARTICLE_DRY_ICE = 0x16,
  PARTICLE_TEARGAS = 0x17,
  PARTICLE_GARAGEPAINT_SPRAY = 0x18,
  PARTICLE_SHARD = 0x19,
  PARTICLE_SPLASH = 0x1A,
  PARTICLE_CARFLAME = 0x1B,
  PARTICLE_STEAM = 0x1C,
  PARTICLE_STEAM2 = 0x1D,
  PARTICLE_STEAM_NY = 0x1E,
  PARTICLE_STEAM_NY_SLOWMOTION = 0x1F,
  PARTICLE_GROUND_STEAM = 0x20,
  PARTICLE_ENGINE_STEAM = 0x21,
  PARTICLE_RAINDROP = 0x22,
  PARTICLE_RAINDROP_SMALL = 0x23,
  PARTICLE_RAIN_SPLASH = 0x24,
  PARTICLE_RAIN_SPLASH_BIGGROW = 0x25,
  PARTICLE_RAIN_SPLASHUP = 0x26,
  PARTICLE_WATERSPRAY = 0x27,
  PARTICLE_WATERDROP = 0x28,
  PARTICLE_BLOODDROP = 0x29,
  PARTICLE_EXPLOSION_MEDIUM = 0x2A,
  PARTICLE_EXPLOSION_LARGE = 0x2B,
  PARTICLE_EXPLOSION_MFAST = 0x2C,
  PARTICLE_EXPLOSION_LFAST = 0x2D,
  PARTICLE_CAR_SPLASH = 0x2E,
  PARTICLE_BOAT_SPLASH = 0x2F,
  PARTICLE_BOAT_THRUSTJET = 0x30,
  PARTICLE_WATER_HYDRANT = 0x31,
  PARTICLE_WATER_CANNON = 0x32,
  PARTICLE_EXTINGUISH_STEAM = 0x33,
  PARTICLE_PED_SPLASH = 0x34,
  PARTICLE_PEDFOOT_DUST = 0x35,
  PARTICLE_CAR_DUST = 0x36,
  PARTICLE_HELI_DUST = 0x37,
  PARTICLE_HELI_ATTACK = 0x38,
  PARTICLE_ENGINE_SMOKE = 0x39,
  PARTICLE_ENGINE_SMOKE2 = 0x3A,
  PARTICLE_CARFLAME_SMOKE = 0x3B,
  PARTICLE_FIREBALL_SMOKE = 0x3C,
  PARTICLE_PAINT_SMOKE = 0x3D,
  PARTICLE_TREE_LEAVES = 0x3E,
  PARTICLE_CARCOLLISION_DUST = 0x3F,
  PARTICLE_CAR_DEBRIS = 0x40,
  PARTICLE_BIRD_DEBRIS = 0x41,
  PARTICLE_HELI_DEBRIS = 0x42,
  PARTICLE_EXHAUST_FUMES = 0x43,
  PARTICLE_RUBBER_SMOKE = 0x44,
  PARTICLE_BURNINGRUBBER_SMOKE = 0x45,
  PARTICLE_BULLETHIT_SMOKE = 0x46,
  PARTICLE_GUNSHELL_FIRST = 0x47,
  PARTICLE_GUNSHELL = 0x48,
  PARTICLE_GUNSHELL_BUMP1 = 0x49,
  PARTICLE_GUNSHELL_BUMP2 = 0x4A,
  PARTICLE_ROCKET_SMOKE = 0x4B,
  PARTICLE_TEST = 0x4C,
  PARTICLE_BIRD_FRONT = 0x4D,
  PARTICLE_SHIP_SIDE = 0x4E,
  PARTICLE_BEASTIE = 0x4F,
  PARTICLE_RAINDROP_2D = 0x50,
  PARTICLE_HEATHAZE = 0x51,
  PARTICLE_HEATHAZE_IN_DIST = 0x52,
  MAX_PARTICLES = 0x53,
};


class CEntity;

class CParticle
{
public:
	CVector   m_vecPosition;
	CVector   m_vecVelocity;
	//CVector   m_vecScreenPosition;
	UInt32     m_nTimeWhenWillBeDestroyed; // Int32
	Int32     m_nTimeWhenColorWillBeChanged;
	Float     m_fZGround;
	CVector   m_vecParticleMovementOffset;
	Int16     m_nCurrentZRotation;
	UInt16    m_nZRotationTimer;
	Float     m_fCurrentZRadius;
	UInt16    m_nZRadiusTimer;
	UInt8     m_nColorIntensity;
	UInt8     m_nAlpha;
	Float     m_fSize;
	Float     m_fExpansionRate;
	UInt16    m_nFadeToBlackTimer;
	UInt16    m_nFadeAlphaTimer;
	Int16     m_nAnimationSpeedTimer;
	Int16     m_nRotationStep;
	Int16     m_nRotation;
	UInt8     m_nCurrentFrame;
	RwRGBA    m_Color;
	char _pad1[1];
	CParticle *m_pNext;
	
	static void ReloadConfig();
	static void Initialise();
	static void Shutdown();
	static void AddParticlesAlongLine(tParticleType type, CVector const &vecStart, CVector const &vecEnd, CVector const &vecDir, Float fPower, CEntity *pEntity, Float fSize, Int32 nRotationSpeed, Int32 nRotation, Int32 nCurFrame, Int32 nLifeSpan);
	static CParticle* AddParticle(tParticleType type, CVector const& vecPos, CVector const& vecDir, CEntity* pEntity, float fSize, int nRotationSpeed, int nRotation, int nCurFrame, int nLifeSpan);
	static CParticle* AddParticle(tParticleType type, CVector const& vecPos, CVector const& vecDir, CEntity* pEntity, float fSize, RwRGBA const& color, int nRotationSpeed, int nRotation, int nCurFrame, int nLifeSpan);
	static void Update();
	static void Render();
	static void RemovePSystem(tParticleType type);
	static void RemoveParticle(CParticle* pParticle, CParticle* pPrevParticle, tParticleSystemData* pPSystemData);
	
	
	static void HandleShipsAtHorizonStuff();
	static void HandleShootableBirdsStuff(CEntity *entity, CVector const&camPos);
	
	static CParticle *&m_pUnusedListHead;
	
	static float *m_CosTable;
	static float *m_SinTable;
};

VALIDATE_SIZE(CParticle, 0x58);