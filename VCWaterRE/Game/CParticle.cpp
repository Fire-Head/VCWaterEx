#include "CParticle.h"

CParticle *&CParticle::m_pUnusedListHead = *(CParticle **)0x98002C;

float *CParticle::m_CosTable = (float *)0x7EFE50;
float *CParticle::m_SinTable = (float *)0x943088;	

// Converted from static void __cdecl CParticle::ReloadConfig() 0x50C430
void CParticle::ReloadConfig()
{
	((void (__cdecl *)())0x565940)();
}

// Converted from static void __cdecl CParticle::Initialise() 0x50C570
void CParticle::Initialise()
{
	((void (__cdecl *)())0x564EC0)();
}

// Converted from static void __cdecl CParticle::Shutdown() 0x50CF40
void CParticle::Shutdown()
{
	((void (__cdecl *)())0x564AE0)();
}

void CParticle::AddParticlesAlongLine(tParticleType type, CVector const &vecStart, CVector const &vecEnd, CVector const &vecDir, Float fPower, CEntity *pEntity, Float fSize, Int32 nRotationSpeed, Int32 nRotation, Int32 nCurFrame, Int32 nLifeSpan)
{
	((void (__cdecl *)(tParticleType, CVector const &, CVector const &, CVector const &, Float, CEntity *, Float, Int32, Int32, Int32, Int32))0x564940)(type, vecStart, vecEnd, vecDir, fPower, pEntity, fSize, nRotationSpeed, nRotation, nCurFrame, nLifeSpan);
}

// Converted from static CParticle * __cdecl CParticle::AddParticle(tParticleType type, CVector const &vecPos, CVector const &vecDir, CEntity *pEntity, Float fSize, Int32 nRotationSpeed, Int32 nRotation, Int32 nCurFrame, Int32 nLifeSpan) 0x50D140
CParticle* CParticle::AddParticle(tParticleType type, CVector const& vecPos, CVector const& vecDir, CEntity* pEntity, float fSize, int nRotationSpeed, int nRotation, int nCurFrame, int nLifeSpan)
{
	return ((CParticle* (__cdecl *)(tParticleType, CVector const&, CVector const&, CEntity*, float, int, int, int, int))0x5648F0)(type, vecPos, vecDir, pEntity, fSize, nRotationSpeed, nRotation, nCurFrame, nLifeSpan);
}

// Converted from static CParticle * __cdecl CParticle::AddParticle(tParticleType type, CVector const &vecPos, CVector const &vecDir, CEntity *pEntity, Float fSize, RwRGBA const &color, Int32 nRotationSpeed, Int32 nRotation, Int32 nCurFrame, Int32 nLifeSpan) 0x50D190
CParticle* CParticle::AddParticle(tParticleType type, CVector const& vecPos, CVector const& vecDir, CEntity* pEntity, float fSize, RwRGBA const& color, int nRotationSpeed, int nRotation, int nCurFrame, int nLifeSpan)
{
	return ((CParticle* (__cdecl *)(tParticleType, CVector const&, CVector const&, CEntity*, float, RwRGBA const&, int, int, int, int))0x563CC0)(type, vecPos, vecDir, pEntity, fSize, color, nRotationSpeed, nRotation, nCurFrame, nLifeSpan);
}

// Converted from static void __cdecl CParticle::Update() 0x50DCF0
void CParticle::Update()
{
	((void (__cdecl *)())0x561E10)();
}

// Converted from static void __cdecl CParticle::Render() 0x50EE20
void CParticle::Render()
{
	((void (__cdecl *)())0x5608C0)();
}

// Converted from static void __cdecl CParticle::RemovePSystem(tParticleType type) 0x50F6E0
void CParticle::RemovePSystem(tParticleType type)
{
	((void (__cdecl *)(tParticleType))0x560870)(type);
}

// Converted from static void __cdecl CParticle::RemoveParticle(CParticle *pParticle, CParticle *pPrevParticle, tParticleSystemData *pPSystemData) 0x50F720
void CParticle::RemoveParticle(CParticle* pParticle, CParticle* pPrevParticle, tParticleSystemData* pPSystemData)
{
	((void (__cdecl *)(CParticle*, CParticle*, tParticleSystemData*))0x560830)(pParticle, pPrevParticle, pPSystemData);
}

void CParticle::HandleShipsAtHorizonStuff()
{
	((void (__cdecl *)())0x5BC440)();
}

void CParticle::HandleShootableBirdsStuff(CEntity *entity, CVector const&camPos)
{
	((void (__cdecl *)(CEntity *, CVector const&))0x5D0180)(entity, camPos);
}
