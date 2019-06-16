#include "CParticleObject.h"

CParticleObject (&gPObjectArray)[MAX_PARTICLEOBJECTS] = *(CParticleObject(*)[MAX_PARTICLEOBJECTS])(*(int*)0x705584);

CParticleObject *&CParticleObject::pCloseListHead = *(CParticleObject **)0x9B48AC;
CParticleObject *&CParticleObject::pFarListHead = *(CParticleObject **)0xA0DAF8;
CParticleObject *&CParticleObject::pUnusedListHead = *(CParticleObject **)0xA0D870;

// Converted from static void __cdecl CParticleObject::Initialise() 0x42C760
void CParticleObject::Initialise()
{
	((void (__cdecl *)())0x4E8970)();
}

// Converted from static void __cdecl CParticleObject::UpdateAll() 0x4BCA30
void CParticleObject::UpdateAll()
{
	((void (__cdecl *)())0x4E8300)();
}

// Converted from static CParticleObject * __cdecl CParticleObject::AddObject(unsigned short Type, CVector const &vecPos, CVector const &vecTarget, float fSize, unsigned int nLifeTime, RwRGBA const &Color, unsigned char bCanRemove) 0x4BC5B0
CParticleObject* CParticleObject::AddObject(unsigned short Type, CVector const& vecPos, CVector const& vecTarget, float fSize, unsigned int nLifeTime, RwRGBA const& Color, unsigned char bCanRemove)
{
	return ((CParticleObject* (__cdecl *)(unsigned short, CVector const&, CVector const&, float, unsigned int, RwRGBA const&, unsigned char))0x4E8480)(Type, vecPos, vecTarget, fSize, nLifeTime, Color, bCanRemove);
}
