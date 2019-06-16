#pragma once
#include "types.h"


class CFire
{
public:
	Bool m_bActive;
	Bool m_bCreatedByScript;
	Bool propogationFlag;
	Bool m_bScriptAudio;
	CVector m_vecPosition;
	CEntity *m_pEntityTarget;
	CEntity *m_pEntityCreator;
	UInt32 m_nTimeToBurn;
	UInt32 m_nTime;
	Int32 m_nNextParticleCreateTime;
	Int32 dword24;
	Int32 dword28;
	Float m_fStrength;
};
VALIDATE_SIZE(CFire, 0x30);