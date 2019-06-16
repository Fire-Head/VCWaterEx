#pragma once

#include "CParticle.h"
#include "..\address.h"


class CReplay
{
public:
	static unsigned char &Mode;
	
	static void RecordParticle(tParticleType type, CVector const &vecPos, CVector const &vecDir, float fSize, RwRGBA const&color);
};