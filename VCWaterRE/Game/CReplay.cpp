#include "CReplay.h"

unsigned char &CReplay::Mode = *(unsigned char *)0x624600;	
	
void CReplay::RecordParticle(tParticleType type, CVector const &vecPos, CVector const &vecDir, float fSize, RwRGBA const&color)
{
	return ((void (__cdecl *)(tParticleType, CVector const &, CVector const &, float, RwRGBA const&))0x624600)(type, vecPos, vecDir, fSize, color);
}
